#include "raylib.h"

int main()
{
    // window variables
    int width{800};
    int height{450};

    InitWindow(width, height, "Axe Game");

    // Circle variables
    int circle_x{200};
    int circle_y{height / 2};
    int circle_radius{25};
    // circle edges
    int l_circ_x{circle_x - circle_radius};
    int r_circ_x{circle_x + circle_radius};
    int u_circ_y{circle_y - circle_radius};
    int b_circ_y{circle_y + circle_radius};

    // axe variables
    int axe_x{400};
    int axe_y{0};
    int axe_width{50};
    int axe_height{50};
    int direction{10};
    // axe edges
    int l_axe_x{axe_x};
    int r_axe_x{axe_x + axe_width};
    int u_axe_y{axe_y};
    int b_axe_y{axe_y + axe_height};

    bool collision_with_axe = (b_axe_y >= u_circ_y) && (u_axe_y <= b_circ_y) && (r_axe_x >= l_circ_x) && (l_axe_x <= r_circ_x);

    SetTargetFPS(60);

    while (WindowShouldClose() != true)
    {
        // setup canvas
        BeginDrawing();

        // clear background to stop flickering
        ClearBackground(WHITE);
        if (collision_with_axe)
        {
            DrawText("Game OVER!", (width / 2) - 35, (height / 2) - 10, 20, RED);
        }
        else
        {
            // GAME LOGIC BEGINS

            // update the edges
            l_circ_x = circle_x - circle_radius;
            r_circ_x = circle_x + circle_radius;
            u_circ_y = circle_y - circle_radius;
            b_circ_y = circle_y + circle_radius;
            l_axe_x = axe_x;
            r_axe_x = axe_x + axe_width;
            u_axe_y = axe_y;
            b_axe_y = axe_y + axe_height;

            // updates collision
            collision_with_axe = (b_axe_y >= u_circ_y) && (u_axe_y <= b_circ_y) && (r_axe_x >= l_circ_x) && (l_axe_x <= r_circ_x);

            // draws circle
            DrawCircle(circle_x, circle_y, circle_radius, BLUE);

            // draws axe
            DrawRectangle(axe_x, axe_y, axe_width, axe_height, RED);

            // bounce the axe on the y edges of the window
            axe_y += direction;
            if (axe_y > height - axe_height || axe_y < 0)
            {
                direction = -direction;
            }

            // moves circle
            if (IsKeyDown(KEY_D) && circle_x < width - circle_radius)
            {
                circle_x += 10;
            }
            if (IsKeyDown(KEY_A) && circle_x > 0 + circle_radius)
            {
                circle_x -= 10;
            }
            if (IsKeyDown(KEY_W) && circle_y > 0 + circle_radius)
            {
                circle_y -= 10;
            }
            if (IsKeyDown(KEY_S) && circle_y < height - circle_radius)
            {
                circle_y += 10;
            }

            // GAME LOGIC ENDS
        }
        EndDrawing();
    }
}