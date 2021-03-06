#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);;
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    previousPosition = position;

    speed = 0;

    modelMatrix = glm::mat4(1.0f);
    loseLife = false;
    collectCoin = false;

    //jumpTime = 0.0f;
}

bool Entity::CheckCollision(Entity* other) {

    if (other == this) return false; // you shouldn't be able to collide with yourself

    //if (entityType == ENEMY && other->entityType == ENEMY) return false;


    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);



    if (xdist < 0 && ydist < 0) {
        return true;
    }

    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            
            

                if (velocity.y > 0) {
                    //position.y -= penetrationY;
                    velocity.y = 0;
                    collidedTop = true;

                }
                else if (velocity.y < 0) {
                    //position.y += penetrationY;
                    velocity.y = 0;
                    collidedBottom = true;
                }

                if (entityType == PLAYER && object->entityType == ENEMY) {
                    position = previousPosition;
                    loseLife = true;
                }
            

            
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
           

                if (velocity.x > 0) {
                    //position.x -= penetrationX;
                    velocity.x = 0;
                    collidedRight = true;
                }
                else if (velocity.x < 0) {
                    //position.x += penetrationX;
                    velocity.x = 0;
                    collidedLeft = true;
                }

                if (entityType == PLAYER && object->entityType == ENEMY) {
                    position = previousPosition;
                    loseLife = true;
                }
            


            
        }
    }
}

void Entity::CheckCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        //position.y -= penetration_y;
        position = previousPosition;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        //position.y -= penetration_y;
        position = previousPosition;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        //position.y -= penetration_y;
        position = previousPosition;
        velocity.y = 0;
        collidedTop = true;
    }
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        //position.y += penetration_y;
        position = previousPosition;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        //position.y += penetration_y;
        position = previousPosition;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        //position.y += penetration_y;
        position = previousPosition;
        velocity.y = 0;
        collidedBottom = true;
    }
}

void Entity::CheckCollisionsX(Map* map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        //position.x += penetration_x;
        position = previousPosition;
        velocity.x = 0;
        collidedLeft = true;
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        //position.x -= penetration_x;
        position = previousPosition;
        velocity.x = 0;
        collidedRight = true;
    }
}


void Entity::AIWalkerX() {
    if (collidedLeft) { //Move Right
        movement = glm::vec3(.75, 0, 0);
        animIndices = animRight;
    }
    else if (collidedRight) { //Move Left
        movement = glm::vec3(-.75, 0, 0);
        animIndices = animLeft;
    }
}

void Entity::AIWalkerY() {
    if (collidedBottom) { //Move Up
        movement = glm::vec3(0, .75, 0);
        animIndices = animUp;
    }
    else if (collidedTop) { //Move Down
        movement = glm::vec3(0, -.75, 0);
        animIndices = animDown;
    }
}

void Entity::AI() {
    switch (aiType) {
        case XWALKER:
            AIWalkerX();
            break;

        case YWALKER:
            AIWalkerY();
            break;
    }
}


void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Entity* coins, int coinCount, Map* map)
{
    if (isActive == false) return;

    previousPosition = position;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    loseLife = false;
    collectCoin = false;

    if (animIndices != NULL) {
        if (glm::length(movement) != 0 || entityType == COIN) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        }
        else {
            animIndex = 0;

            if (entityType == PLAYER && animIndices == animLeft) {
                animIndex = 3;
            }
        }
    }

    velocity.x = movement.x * speed; //this is so the character has instant velocity
    velocity.y = movement.y * speed;
    velocity += acceleration * deltaTime; //if we are moving, we are going to keep adding to velocity

    position.y += velocity.y * deltaTime; // Move on Y
    CheckCollisionsY(map);
    CheckCollisionsY(objects, objectCount); // Fix if needed
    CheckCollisionsY(coins, coinCount);

    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(map);
    CheckCollisionsX(objects, objectCount); // Fix if needed
    CheckCollisionsX(coins, coinCount);

    if (entityType == ENEMY) {
        AI();
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) {

    if (isActive == false) return;

    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
