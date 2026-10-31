#include "raylib.h"
#include "raymath.h"
#include <cstddef>
#include <vector>
#include <cmath>

struct Segment
{
    Vector2 position;
    float angle = 0.0f;
    float length;

    Segment(float x, float y, float len) : position{x, y}, length(len) {}

    Vector2 getA() const
    {
        return position;
    }

    Vector2 getB() const
    {
        return {
            position.x + length * cosf(angle),
            position.y + length * sinf(angle)};
    }

    void follow(Vector2 target)
    {
        Vector2 dir = {target.x - position.x, target.y - position.y};
        angle = atan2f(dir.y, dir.x);

        position.x = target.x - length * cosf(angle);
        position.y = target.y - length * sinf(angle);
    }
};

class Creature
{
public:
    std::vector<Segment> body;
    int totalSegments;
    float segmentLength;
    Vector2 smoothedTarget{};

    Creature(int count, float length, float startX, float startY)
        : totalSegments(count), segmentLength(length)
    {
        for (int i = 0; i < totalSegments; i++)
        {
            body.emplace_back(startX - i * length, startY, length);
        }
        smoothedTarget = {startX, startY};
    }

    void update(Vector2 target)
    {
        smoothedTarget = Vector2Lerp(smoothedTarget, target, 0.35f);

        body[0].follow(smoothedTarget);

        for (std::size_t i = 1; i < body.size(); i++)
        {
            body[i].follow(body[i - 1].getA());
        }
    }

    void draw()
    {
        int n = static_cast<int>(body.size());
        for (int i = 0; i < n; i++)
        {
            Vector2 start = body[i].getA();
            Vector2 end = body[i].getB();

            DrawLineEx(start, end, 3.0f, WHITE);
            DrawCircleV(start, 4.0f, LIGHTGRAY);

            if (i % 2 == 0 && i > 0 && i < n - 2)
            {
                float perpAngle1 = body[i].angle + PI / 2.0f;
                float perpAngle2 = body[i].angle - PI / 2.0f;
            }
        }
    }
};

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Cursor Scorpion - Raylib");
    SetTargetFPS(60);

    Creature scorpion(20, 15.0f, screenWidth / 2.0f, screenHeight / 2.0f);

    while (!WindowShouldClose())
    {
        Vector2 mousePos = GetMousePosition();
        scorpion.update(mousePos);

        BeginDrawing();
        ClearBackground(BLACK);

        scorpion.draw();

        DrawText("Move the mouse to control the scorpion", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}