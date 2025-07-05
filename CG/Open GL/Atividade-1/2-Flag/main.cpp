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
#include <algorithm> // std::clamp

template <typename T>
constexpr T M_TAU = static_cast<T>(2 * M_PI);

struct Vec2 {
    float x, y;
    Vec2() : x(0), y(0) {} // implicit constructor
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2(const Vec2& v) : x(v.x), y(v.y) {} // copy
    Vec2(Vec2&& v) : x(v.x), y(v.y) {} // move
    Vec2 operator/(float f) const { return {x / f, y / f}; }
    Vec2 operator*(float f) const { return {x * f, y * f}; }
    Vec2 operator+(Vec2 v) const { return {x + v.x, y + v.y}; }
    Vec2 operator-(Vec2 v) const { return {x - v.x, y - v.y}; }
};

struct Vec3 {
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {} // implicit constructor
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) {} // copy
    Vec3(Vec3&& v) : x(v.x), y(v.y), z(v.z) {} // move
    Vec3 operator/(float f) const { return {x / f, y / f, z / f}; }
    Vec3 operator*(float f) const { return {x * f, y * f, z * f}; }
    Vec3 operator+(Vec3 v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator-(Vec3 v) const { return {x - v.x, y - v.y, z - v.z}; }
};


struct Color {
    unsigned char r, g, b;

    Color() : r(0), g(0), b(0) {} // implicit constructor
    Color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
    Vec3 normalized() const { return Vec3(r / 255.0f, g / 255.0f, b / 255.0f); }
};

const Vec2 FLAG_SIZE{20, 14};
const Vec2 WINDOW_SIZE{FLAG_SIZE * 30}; // aspect ratio: 10:7

struct FlagColors {
    static const Color GREEN, YELLOW, BLUE, WHITE;
};
const Color FlagColors::GREEN{0, 148, 64};
const Color FlagColors::YELLOW{255, 203, 0};
const Color FlagColors::BLUE{48, 38, 129};
const Color FlagColors::WHITE{255, 255, 255};

//#define _ENABLE_ZOOM 1 // para debug


/* Inicialização do renderer */
int init(void)
{
    auto [r, g, b] = FlagColors::GREEN.normalized();
    glClearColor(r, g, b, 1.0f); // cor de fundo

    glMatrixMode(GL_PROJECTION);

#ifdef _ENABLE_ZOOM
    Vec2 center = FLAG_SIZE / 2.0f;
    Vec2 halfSize = FLAG_SIZE / 4.0f; // metade da metade → zoom 2x

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // sempre limpe antes de aplicar nova projeção
    gluOrtho2D(
        center.x - halfSize.x,
        center.x + halfSize.x,
        center.y - halfSize.y,
        center.y + halfSize.y
    );
#else
    gluOrtho2D(0.0, FLAG_SIZE.x, 0.0, FLAG_SIZE.y); // coordenadas limite do viewport normalizadas (em 2D)
#endif

    return EXIT_SUCCESS;
}

// Referência para a construção matemática da bandeira: <https://youtu.be/yBjX9jLuLSY>
void display()
{
    inline void genCircleAuto(Vec2 center, float radius, float quality = 1.5f, int min_offset = 12, int limit = 2048);
    inline void genSemiArc(Vec2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, std::size_t segmentsByArc);

    inline void genSemiArcOverCircle( const Vec2& arcCenter, float innerRadius, float outerRadius,
            const Vec2& circleCenter, float circleRadius, std::size_t segments = 64, std::size_t edgeSegments = 4);

    /* Loop principal de desenho */
    glClear(GL_COLOR_BUFFER_BIT); // Limpa o buffer de cor

    glColor3ub(FlagColors::YELLOW.r, FlagColors::YELLOW.g, FlagColors::YELLOW.b);

    // Desenha o diamante (losango) da bandeira do Brasil
    float diamondOffset = 1.7f;
    glBegin(GL_POLYGON);
        glVertex2f(FLAG_SIZE.x / 2.0f, FLAG_SIZE.y - diamondOffset);
        glVertex2f(FLAG_SIZE.x - diamondOffset, FLAG_SIZE.y / 2.0f);
        glVertex2f(FLAG_SIZE.x / 2.0f, diamondOffset);
        glVertex2f(diamondOffset, FLAG_SIZE.y / 2.0f);
    glEnd();

    // Desenha o círculo da bandeira do Brasil
    Vec2 center{FLAG_SIZE / 2.0f};
    float radius = 3.5f;

    glColor3ub(FlagColors::BLUE.r, FlagColors::BLUE.g, FlagColors::BLUE.b);
    genCircleAuto(center, radius, 10.0f);

    // Desenha a faixa da bandeira, composta por arcos
    glColor3ub(FlagColors::WHITE.r, FlagColors::WHITE.g, FlagColors::WHITE.b);
    Vec2 arcCenter{center.x - 2.0f, 0.0f};
    genSemiArcOverCircle(
        arcCenter,
        8.0f,     // innerRadius
        8.5f,     // outerRadius
        center,   // circleCenter (10,7)
        3.5f,     // circleRadius
        32       // número de segmentos (suavidade)
    );

    glFlush(); // desenha comandos não executados, forçando sua execução em tempo finito
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Inicializa o GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_MULTISAMPLE); // modo de exibição: frame buffer, modelo de cor, e antialias ativado
    glEnable(GL_MULTISAMPLE);
    glutInitWindowSize(WINDOW_SIZE.x, WINDOW_SIZE.y); // tamanho da área interna da janela (coordenadas de tela)
    glutCreateWindow("Trabalho CG [Open GL]: Bandeira do Brasil");

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
    float offset = M_TAU<float> / segments;
    float angle = 0.0f; // current angle in radians
    for (std::size_t i = 0; i < segments; i++) {
        // define a coordenada de cada ponto no perímetro do círculo
        x = center.x + radius * cosf(angle);
        y = center.y + radius * sinf(angle);

        glVertex2f(x, y);
        angle += offset;
    }
    glEnd();
}

/* Gera um semi-círculo em Open GL frame buffer
 * segmentsByArc: quantidade de segmentos em cada arco (interno e externo)
 */
inline void genSemiArc(Vec2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, std::size_t segmentsByArc) {
    glBegin(GL_LINE_LOOP); // Inicializa o desenho de um polígono
    float x, y;
    float offset = (endAngle - startAngle) / segmentsByArc;
    float angle = startAngle; // current angle in radians

    // Arco externo
    for (std::size_t i = 0; i <= segmentsByArc; i++) {
        // define a coordenada de cada ponto no perímetro do semi-arco
        x = center.x + outerRadius * cosf(angle);
        y = center.y + outerRadius * sinf(angle);

        glVertex2f(x, y);
        angle += offset;
    }

    glColor3ub(0, 0, 0);
    // Arco interno
    for (std::size_t i = 0; i <= segmentsByArc; i++) {
        // define a coordenada de cada ponto no perímetro do semi-arco
        x = center.x + innerRadius * cosf(angle);
        y = center.y + innerRadius * sinf(angle);

        glVertex2f(x, y);
        angle -= offset;
    }
    glEnd();
}

#include <random>
/**
 * Gera um "anel" parcial (faixa) entre dois raios (innerRadius, outerRadius)
 * de um centro arcCenter, mas somente o trecho que fica DENTRO do círculo
 * (circleCenter, circleRadius).
 */
inline void genSemiArcOverCircle(
    const Vec2& arcCenter,
    float innerRadius,
    float outerRadius,
    const Vec2& circleCenter,
    float circleRadius,
    std::size_t segments    = 64,
    std::size_t edgeSegments = 4
) {
    // 1) distância e direção entre centros
    float dx = circleCenter.x - arcCenter.x;
    float dy = circleCenter.y - arcCenter.y;
    float d  = std::sqrt(dx*dx + dy*dy);
    if (d < 1e-6f) return;
    float phi = std::atan2(dy, dx);

    // 2) intervalo de ângulos no arco interno/externo (lei dos cossenos)
    auto computeInterval = [&](float R, float& t0, float& t1){
        float cosA = (d * d + R * R - circleRadius * circleRadius) / (2.0f * d * R);
        cosA = std::clamp(cosA, -1.0f, 1.0f);
        float alpha = std::acos(cosA);
        t0 = phi - alpha;
        t1 = phi + alpha;
    };
    float in0, in1, out0, out1;
    computeInterval(innerRadius, in0, in1);
    computeInterval(outerRadius, out0, out1);

    // 3) ângulos de interseção NO PERÍMETRO DO CÍRCULO AZUL,
    //    usando o raio correto para cada t
    auto intersectAng = [&](float R, float theta){
        Vec2 p{ arcCenter.x + R * std::cos(theta),
                arcCenter.y + R * std::sin(theta) };
        return std::atan2(p.y - circleCenter.y,
                          p.x - circleCenter.x);
    };
    float angIn0  = intersectAng(innerRadius, in0);
    float angOut0 = intersectAng(outerRadius, out0);
    float angIn1  = intersectAng(innerRadius, in1);
    float angOut1 = intersectAng(outerRadius, out1);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Modo wireframe

    // 4) monta o TRIANGLE_STRIP
    glBegin(GL_TRIANGLE_STRIP);

    // 4.1) lateral esquerda: interpolar de angOut0 → angIn0
    for (std::size_t i = 0; i <= edgeSegments; ++i) {
        float t    = float(i) / edgeSegments;
        float aOut = angOut0*(1 - t) + angIn0*t;
        float aIn  = angOut0*(1 - t) + angIn0*t;
        // ponto na borda externa (círculo azul)
        glVertex2f(
            circleCenter.x + circleRadius * std::cos(aOut),
            circleCenter.y + circleRadius * std::sin(aOut)
        );
        // ponto na borda interna (também no círculo azul)
        glVertex2f(
            circleCenter.x + circleRadius * std::cos(aIn),
            circleCenter.y + circleRadius * std::sin(aIn)
        );
    }

    // 4.2) arcos principal (outer → inner) no centro da faixa
    for (std::size_t i = 0; i <= segments; ++i) {
        float to = out0 + (out1 - out0) * (float(i) / segments);
        float ti = in0  + (in1 - in0) * (float(i) / segments);
        // externo
        glVertex2f(
            arcCenter.x + outerRadius * std::cos(to),
            arcCenter.y + outerRadius * std::sin(to)
        );
        // interno
        glVertex2f(
            arcCenter.x + innerRadius * std::cos(ti),
            arcCenter.y + innerRadius * std::sin(ti)
        );
    }

    // 4.3) lateral direita: interpolar de angIn1 → angOut1
    for (std::size_t i = 0; i <= edgeSegments; ++i) {
        float t    = float(i) / edgeSegments;
        float aOut = angIn1 * (1 - t) + angOut1*t;
        float aIn  = angIn1 * (1 - t) + angOut1*t;
        glVertex2f(
            circleCenter.x + circleRadius * std::cos(aOut),
            circleCenter.y + circleRadius * std::sin(aOut)
        );
        glVertex2f(
            circleCenter.x + circleRadius * std::cos(aIn),
            circleCenter.y + circleRadius * std::sin(aIn)
        );
    }

    glEnd();
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
