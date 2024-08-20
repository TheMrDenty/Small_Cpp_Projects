#include "raylib.h"

struct AnimData
{
    Rectangle rect;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rect.height;
};

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // updating running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // update animation frame
        data.rect.x = data.frame * data.rect.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
};

int main()
{
    // window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    // initialize window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher!");

    // acceleration due to gravity (pixels/s)/s
    const int gravity{1'000};

    // nebula sprite
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{10};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rect.x = 0.0;
        nebulae[i].rect.y = 0.0;
        nebulae[i].rect.width = nebula.width / 8;
        nebulae[i].rect.height = nebula.height / 8;
        nebulae[i].pos.x = windowDimensions[0] + (300 * i);
        nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0 / 16.0;
    }

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

    // nebula x velocity (pixels/s)
    int nebVel{-300};

    // scarfy sprite
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData{
        // 6 textures so divide width by 6
        {scarfyData.rect.x = 0, scarfyData.rect.y = 0, scarfyData.rect.width = scarfy.width / 6, scarfyData.rect.height = scarfy.height},
        {scarfyData.pos.x = (windowDimensions[0] / 2) - (scarfyData.rect.width / 2), scarfyData.pos.y = windowDimensions[1] - scarfyData.rect.height},
        scarfyData.frame = 0,
        scarfyData.updateTime = 1.0 / 12.0,
        scarfyData.runningTime = 0.0};

    int velocity{0};

    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float bgX{};
    float mgX{};
    float fgX{};

    bool isInAir;
    // jump velocity (pixels/s)
    const int jumpVelocity{-500};
    bool collision{};
    SetTargetFPS(60);
    // game loop
    while (!WindowShouldClose())
    {
        // delta time (time since last frame)
        const float deltaTime{GetFrameTime()};

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * deltaTime;
        if (bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }
        mgX -= 40 * deltaTime;
        if (mgX <= -midground.width * 2)
        {
            mgX = 0.0;
        }
        fgX -= 80 * deltaTime;
        if (fgX <= -foreground.width * 2)
        {
            fgX = 0.0;
        }

        // draw background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);
        // draw midground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);
        // draw foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        if (isOnGround(scarfyData, windowDimensions[1]))
        {
            // on ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // in air
            // apply gravity
            velocity += gravity * deltaTime;
            isInAir = true;
        }

        // jump check
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVelocity;
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            // update nebula position
            nebulae[i].pos.x += nebVel * deltaTime;
        }

        // update finish line
        finishLine += nebVel * deltaTime;

        // update scarfy pos
        scarfyData.pos.y += velocity * deltaTime;

        if (!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, deltaTime, 5);
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], deltaTime, 7);
        }

        for (AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRect{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rect.width - 2 * pad,
                nebula.rect.height - 2 * pad};

            Rectangle scarfyRect{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rect.width,
                scarfyData.rect.height,
            };

            if (CheckCollisionRecs(nebRect, scarfyRect))
            {
                collision = true;
            };
        };
        if (collision)
        {
            // lose game
            DrawText("GAME OVER!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, RED);
        }
        else if (scarfyData.pos.x >= finishLine)
        {
            // win the game
            DrawText("YOU WIN!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, GREEN);
        }
        else
        {
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rect, nebulae[i].pos, WHITE);
            }

            // draw scarfy
            DrawTextureRec(scarfy, scarfyData.rect, scarfyData.pos, WHITE);
        }

        // end drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}