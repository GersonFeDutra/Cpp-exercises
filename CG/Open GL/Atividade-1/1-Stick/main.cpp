#if defined(_WIN32) || defined(_WIN64)
    // Windows
    #include <windows.h>
#elif defined(__APPLE__) || defined(__MACH__)
    static_assert(false, "Não implementado para macOS");
#elif defined(__linux__) || defined(__unix__) || defined(unix) || defined(__unix)
    // Linux
#else
    static_assert(false, "Sistema operacional desconhecido");
#endif
#include <GL/glut.h>
//#include <GL/freeglut.h> // Free GLUT moderno

#include <cstdlib> // Inclui algumas convenções do C
#include <cmath> // Inclui algumas funções matemáticas básicas

struct Vec2 {
    float x, y;
    Vec2 operator/(float f) const { return {x / f, y / f}; }
};

const Vec2 WINDOW_SIZE{640, 480};

/* Inicialização do renderer */
int init(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // cor de fundo

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WINDOW_SIZE.x, 0.0, WINDOW_SIZE.y); // coordenadas limite do viewport normalizadas (em 2D)

    return EXIT_SUCCESS;
}

void display()
{
    inline void genCircleAuto(Vec2 center, float radius, float quality = 1.5f, int min_offset = 12, int limit = 2048);

    /* Loop principal de desenho */
    glClear(GL_COLOR_BUFFER_BIT); // Limpa o buffer de cor

    glColor3ub(225, 10, 55);

    Vec2 head_position = WINDOW_SIZE / 2.0f;
    head_position.y += 100.0f;
    float head_radius = 25.0f;
    float torso_end = WINDOW_SIZE.y / 2.0f - 100.0f;

    genCircleAuto(head_position, head_radius);

    glLineWidth(4.0f);
    glBegin(GL_LINES);
        glVertex2f(head_position.x, head_position.y + head_radius);
        glVertex2f(head_position.x, torso_end);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2f(head_position.x - head_radius * 3, head_position.y - head_radius * 3);
        glVertex2f(head_position.x, head_position.y - head_radius * 2);
        glVertex2f(head_position.x + head_radius * 3, head_position.y - head_radius * 3);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2f(head_position.x - head_radius * 3, torso_end - head_radius * 3);
        glVertex2f(head_position.x, torso_end);
        glVertex2f(head_position.x + head_radius * 3, torso_end - head_radius * 3);
    glEnd();

    glFlush(); // desenha comandos não executados, forçando sua execução em tempo finito
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Inicializa o GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_MULTISAMPLE); // modo de exibição: frame buffer, modelo de cor, e antialias ativado
    glEnable(GL_MULTISAMPLE);
    glutInitWindowSize(WINDOW_SIZE.x, WINDOW_SIZE.y); // tamanho da área interna da janela (coordenadas de tela)
    glutCreateWindow("Open GL: Olá mundo!");

    int err = init();
    if (err != EXIT_SUCCESS)
        return err;

    glutDisplayFunc(display); // Estabelece a callback de exibição
    glutMainLoop(); // Mostre tudo, e espere

    return EXIT_SUCCESS;
}


/* Generates a circle in Open GL frame buffer */
inline void genCircle(Vec2 center, float radius, std::size_t segments) {
    glBegin(GL_POLYGON); // Inicializa o desenho de um polígono
    float x, y;
    float offset = (float)M_PI / segments;
    for (std::size_t i = 0; i < segments; i++) {
        // define a coordenada de cada ponto no perímetro do círculo
        x = center.x + radius * cosf((i << 1) * offset);
        y = center.y + radius * sinf((i << 1) * offset);
        glVertex2f(x, y);
    }
    glEnd();
}

inline void genCircle(float x, float y, float radius, std::size_t segments) {
    genCircle({x, y}, radius, segments);
}


/* Gera um círculo com base na qualidade.
 * quality: Fator de suavidade ajustável. Quanto maior, mais segmentos.
 * min_offset: Quantidade mínima de segmentos.
 * limit: Quantidade máxima de segmentos.
 */
inline void genCircleAuto(Vec2 center, float radius, float quality = 1.5f, int min_offset = 12, int limit = 2048) {
    int segs = int(quality * radius) + min_offset; // Valor base linear + offset mínimo
    genCircle(center, radius, std::min(segs, limit));
}


inline void genCircleAuto(float x, float y, float radius, float quality = 1.5f, int min_offset = 12, int limit = 2048) {
    genCircleAuto({x, y}, radius, quality, min_offset, limit);
}
