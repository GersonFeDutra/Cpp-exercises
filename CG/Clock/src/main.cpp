// Unix Only!
#include <GL/glut.h>
//#include <GL/freeglut.h> // Free GLUT
#include <cmath> // sin, cos
#include <array> // Vec2
#include <ctime>


#pragma region Type Aliases
    struct Vec2 {
        union {
            struct {
                float x;
                float y;
            };
            std::array<float, 2>coords;
        };
        constexpr Vec2(float x, float y) : x(x), y(y) {}
        constexpr Vec2(int x, int y) : x((float)x), y((float)y) {}
    };
    using Angle = float; // radians
    using Delta = float; // △t
#pragma endregion


#pragma region Global Constants
    template <typename T>
    constexpr T TAU = static_cast<T>(2 * M_PI);

    template <typename T>
    constexpr T PI = static_cast<T>(M_PI);
#pragma endregion

#pragma region Global Variables
    Vec2 window_size{ 800.0f, 800.0f }; // aspect-ratio: 1:1

    int hours = 0;
    int minutes = 0;
    int seconds = 0;
#pragma endregion


#pragma region Inlines

inline std::array<float, 2> angle_to_coords(Vec2 origin, Angle from, float radius)
{
    return std::array<float, 2>{origin.x + radius * cosf(from), origin.y + radius * sinf(from)};
}

inline void draw_circle(Vec2 center, float radius, std::size_t segments)
{
    glBegin(GL_POLYGON);
    float offset = TAU<float> / segments;
    Angle angle = 0.0f;
    for (size_t i = 0; i < segments; ++i) {
        auto [x, y] = angle_to_coords(center, angle, radius);
        glVertex2f(x, y); // coordenada de cada ponto no perímetro do círculo
        angle += offset;
    }
    glEnd();
}

inline void draw_pointer(Vec2 center, float radius, Angle angle)
{
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(center.x, center.y);
    glVertex2f(center.x + radius * cosf(angle), center.y + radius * sinf(angle));
    glEnd();
}

void draw_text(float x, float y, const char *string)
{
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

#pragma endregion // Inlines


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(window_size.x, window_size.y);

    Vec2 screen_size{glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT)};
    glutInitWindowPosition(screen_size.x / 2.0f, screen_size.y / 2.0f);
    glutCreateWindow("Clock");

    void init(void);
    void display(void);
    void reshape(int w, int h);
    void idle(void);

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}

void init(void)
{
    #pragma region Background
    glClearColor(0.03, 0.03, 0.03, 0.03); // Cinza-escuro translúcido
    #pragma endregion

    #pragma region Point-Line Smoothing
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    #pragma endregion
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    #pragma region Draw Clock
        #pragma region Mostrador
        constexpr Vec2 origin = { 0.0f, 0.0f };
        const float radius = 0.9f;
        glColor3f(1.0f, 1.0f, 1.0f);
        draw_circle(origin, radius, 16 + window_size.x * (PI<float> / 4.0f));
        #pragma endregion

        #pragma region Marcação
        const float text_radius = radius * .9f;
        glColor3f(0.0f, 0.0f, 0.0f);
        {
            auto [x, y] = angle_to_coords(origin, PI<float> / 2.0f, text_radius);
            draw_text(x, y, "12");
        }
        {
            auto [x, y] = angle_to_coords(origin, 0.0f, text_radius);
            draw_text(x, y, "3");
        }
        {
            auto [x, y] = angle_to_coords(origin, -PI<float> / 2.0f, text_radius);
            draw_text(x, y, "6");
        }
        {
            auto [x, y] = angle_to_coords(origin, PI<float>, text_radius);
            draw_text(x, y, "9");
        }
        {
            auto [x, y] = angle_to_coords(origin, PI<float> / 6.0f, text_radius);
            draw_text(x, y, "2");
        }
        {
            auto [x, y] = angle_to_coords(origin, 2 * (PI<float> / 6.0f), text_radius);
            draw_text(x, y, "1");
        }
        {
            auto [x, y] = angle_to_coords(origin, -PI<float> / 6.0f, text_radius);
            draw_text(x, y, "4");
        }
        {
            auto [x, y] = angle_to_coords(origin, -2 * (PI<float> / 6.0f), text_radius);
            draw_text(x, y, "5");
        }
        {
            auto [x, y] = angle_to_coords(origin, PI<float> -PI<float> / 6.0f, text_radius);
            draw_text(x, y, "10");
        }
        {
            auto [x, y] = angle_to_coords(origin, PI<float> -2 * (PI<float> / 6.0f), text_radius);
            draw_text(x, y, "11");
        }
        {
            auto [x, y] = angle_to_coords(origin, PI<float> +PI<float> / 6.0f, text_radius);
            draw_text(x, y, "8");
        }
        {
            auto [x, y] = angle_to_coords(origin, PI<float> +2 * (PI<float> / 6.0f), text_radius);
            draw_text(x, y, "7");
        }
        #pragma endregion

        #pragma region Ponteiros
        const float sm_rad_proportion = TAU<float> / 60.0f; // seconds, minutes
        const float h_rad_proportion = TAU<float> / 12.0f; // hours
        const float adjustment = PI<float> / 2.0f; // 0º at +x
        float angle;
        // Segundo
        glColor3f(.0f, .0f, .7f);
        angle = sm_rad_proportion * seconds;
        draw_pointer(origin, radius * .9f, -angle + adjustment);
        // Minuto
        glColor3f(.0f, .7f, .0f);
        angle = sm_rad_proportion * minutes
                + sm_rad_proportion / 60.0f * seconds; // interpolação (sec)
        draw_pointer(origin, radius * .73f, -angle + adjustment);
        // Hora
        glColor3f(.7f, .0f, .0f);
        angle = h_rad_proportion * hours
                + h_rad_proportion / 60.0f * minutes // interpolação (min)
                + h_rad_proportion / 60.0f / 60.0f * seconds; // interpolação (sec)
        draw_pointer(origin, radius * .51f, -angle + adjustment);
        #pragma endregion
    #pragma endregion

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    window_size.x = (float)w;
    window_size.y = (float)h;
    // Mantém a proporção e centralizado
    int min = std::min(w, h);
    int max = std::max(w, h);
    glViewport((w - min) / 2, (h - min) / 2, min, min);
    // projeção padrão é entre -1 e 1
}

void idle(void)
{
    std::time_t current = std::time(0);
    std::tm *local = std::localtime(&current);
    hours = local->tm_hour;
    minutes = local->tm_min;
    seconds = local->tm_sec;

    glutPostRedisplay();
}
