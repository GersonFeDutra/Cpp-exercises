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

struct Vec2 {
    float x, y;
};

const Vec2 WINDOW_SIZE{640, 480};

/* Inicialização do renderer */
int init(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // cor de fundo

    // mantive o padrão de coordenadas normalizadas, pois é mais conveniente para este exercício
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // Matriz de projeção será identidade (padrão)
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // coordenadas limite do viewport normalizadas (em 2D)

    return EXIT_SUCCESS;
}

void display()
{
    /* Loop principal de desenho */
    glClear(GL_COLOR_BUFFER_BIT); // Limpa o buffer de cor

    glBegin(GL_TRIANGLES); // Inicializa o desenho de triângulos
        /* define a cor e coordenada de cada vertex */
        glColor3f(1, 0, 0); glVertex2f(0.5f, -0.5f);
        glColor3f(0, 1, 0); glVertex2f(-0.5f, -0.5f);
        glColor3f(0, 0, 1); glVertex2f(0.0f,  0.5f);
    glEnd();

    glFlush(); // desenha comandos não executados, forçando sua execução em tempo finito
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Inicializa o GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // modo de exibição: frame buffer e modelo de cor
    glutInitWindowSize(WINDOW_SIZE.x, WINDOW_SIZE.y); // tamanho da área interna da janela (coordenadas de tela)
    glutCreateWindow("Open GL: Olá mundo!");

    int err = init();
    if (err != EXIT_SUCCESS)
        return err;

    glutDisplayFunc(display); // Estabelece a callback de exibição
    glutMainLoop(); // Mostre tudo, e espere

    return EXIT_SUCCESS;
}
