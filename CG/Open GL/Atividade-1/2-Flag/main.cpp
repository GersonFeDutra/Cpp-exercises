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
#include <cstring> // Manipulação de cadeias de caracteres
#include <algorithm> // std::clamp
#include <random>


template <typename T>
constexpr T TAU = static_cast<T>(2 * M_PI);

template <typename T>
constexpr T PI = static_cast<T>(M_PI);

struct Vec2 {
    float x = 0.0f, y = 0.0f;

    constexpr Vec2() = default;
    // Construtores
    constexpr Vec2(float x, float y) : x(x), y(y) {}

    // Operadores aritméticos
    constexpr Vec2 operator/(float f) const { return {x / f, y / f}; }
    constexpr Vec2 operator*(float f) const { return {x * f, y * f}; }
    constexpr Vec2 operator+(Vec2 v) const { return {x + v.x, y + v.y}; }
    constexpr Vec2 operator-(Vec2 v) const { return {x - v.x, y - v.y}; }
    Vec2& operator+=(Vec2 v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vec2& operator-=(Vec2 v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    inline float dot() {
        return x * x + y * y;
    }
};


struct Vec3 {
    float x = 0.0f, y = 0.0f, z = 0.0f;

    // Construtores
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Operadores aritméticos
    Vec3 operator/(float f) const { return {x / f, y / f, z / f}; }
    Vec3 operator*(float f) const { return {x * f, y * f, z * f}; }
    Vec3 operator+(Vec3 v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator-(Vec3 v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3& operator+=(const Vec3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vec3& operator-=(const Vec3 &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    inline float dot() {
        return x * x + y * y + z * z;
    }
    inline Vec3 cross() {
        return Vec3(y * z, z * x, x * y);
    }
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
    inline void genCircleAuto(Vec2 center, float radius, float quality = 1.5f,
            int min_offset = 12, int limit = 2048);
    inline void genSemiArc(Vec2 center, float innerRadius, float outerRadius,
            float startAngle, float endAngle, std::size_t segmentsByArc);
    inline void genSemiArcOverCircle(const Vec2& arcCenter, float innerRadius,
            float outerRadius, const Vec2& circleCenter, float circleRadius,
            std::size_t segments = 64, std::size_t edgeSegments = 4);
    inline void genStar(const Vec2& center, float outerRadius, float innerRatio = 0.5f);
    void drawArcText(const char* text, float radius, float startAngle, float arcAngle, float scale);
    std::pair<float, float> computeArcAngles(const Vec2& arcCenter, float radius, const Vec2& circleCenter, float circleRadius);

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
    static constexpr Vec2 CENTER{FLAG_SIZE / 2.0f};
    static constexpr float RADIUS = 3.5f;

    glColor3ub(FlagColors::BLUE.r, FlagColors::BLUE.g, FlagColors::BLUE.b);
    genCircleAuto(CENTER, RADIUS, 10.0f);

    // Desenha a faixa da bandeira, composta por arcos
    glColor3ub(FlagColors::WHITE.r, FlagColors::WHITE.g, FlagColors::WHITE.b);
    const Vec2 arcCenter{CENTER.x - 2.0f, 0.0f};
    const float arcInnerRadius = 8.0f, arcOuterRadius = 8.5f;
    genSemiArcOverCircle( arcCenter,
        arcInnerRadius, arcOuterRadius, // radius: inner, outer
        CENTER, 3.5f,
        32        // número de segmentos (suavidade)
    );

    Vec2 starCoords = CENTER;
    starCoords.y -= 0.5;

    inline void randomizeColor();
    static constexpr float starSizes[] = {.21f, .17f, .13f, .1f, .08f};
    struct Star {
        const float CELL_SCALAR = RADIUS / 10.4f;
        float radius;
        Vec2 coords;

        /**
         * @param size Tamanho da estrela (1-5), referente às posições em starSizes.
         * @param radialCoords Coordenadas em relação ao centro do mapa, numa grade de 1/9.5 do raio por quadrante.
         * Referência: <https://upload.wikimedia.org/wikipedia/commons/b/be/Flag_of_Brazil_%28dimensions%29.svg>.
         */
        Star(uint8_t size, Vec2 radialCoords) : radius(radius), coords(radialCoords) {
            float radius = starSizes[size - 1];

            coords.y -= 0.5f;
            coords = CENTER + coords * CELL_SCALAR;
            genStar(coords, radius, 0.45f);
        }
    };
    /* Estrelas da bandeira do Brasil */
    // Referências: <https://pt.wikipedia.org/wiki/Bandeira_do_Brasil#/media/Ficheiro:Flag_of_Brazil_(sphere).svg> (estrelas)
    // <https://pt.wikipedia.org/wiki/Bandeira_do_Brasil#/media/Ficheiro:Brazil_flag_stars.svg> (constelações)

    //randomizeColor();
    // Virgo
    Star spica(1, {3, 4}); // Pará

    //randomizeColor();
    // Hydra
    Star dhanabAlShuja(3, {2.25f, 1.25f}); // Acre
    Star alphard(2, {-3.75f, -.75f}); // Mato Grosso do Sul

    //randomizeColor();
    // Scorpius
    Star graffias(3, {8.25f, -3.75f}); // Maranhão
    Star antares(1, {6.75f, -3.75f}); // Piauí
    Star wei(2, {7.25f, -4.75f}); // Ceará
    Star shaula(2, {6.75f, -5.4f}); // Rio Grande do Norte
    //randomizeColor();
    Star girtab(3, {5.75f, -5.75f}); // Paraiba
    Star denebakrab(3, {4.75f, -5.75f}); // Pernambuco
    Star sargas(2, {4.75f, -6.75f}); // Alagoas
    Star apollyon(3, {4.75f, -7.75f}); // Sergipe

    //randomizeColor();
    // Triangulum Australe
    Star deltaTrianguliAustralis(3, {3.75f, -6}); // Santa Catarina
    Star atria(2, {2.75f, -7}); // Rio Grande do Sul
    Star gamaTrianguliAustralis(3, {2, -5.75f}); // Paraná

    //randomizeColor();
    // Octans
    Star polarisAustralis(5, {0, -8}); // Distrito Federal

    //randomizeColor();
    // Crux
    Star acrux(1, {0, -4.8f}); // São Paulo
    Star mimosa(2, {1, -2.9f}); // Rio de Janeiro
    Star intrometida(4, {0.5f, -3.5f}); // Espirito Santo
    Star palida(3, {-1.25f, -3}); // Minas Gerais
    Star gacrux(2, {.15f, -2}); // Bahia

    //randomizeColor();
    // Carina
    Star canopus(1, {-4, -5.75f}); // Goiás

    //randomizeColor();
    // Canis Major
    Star adhara(3, {-5.25f, -4.75f}); // Tocantins
    Star wezen(2, {-5, -3.8f}); // Roraima
    Star mirzam(2, {-8, -3.6f}); // Amapá
    Star sirius(1, {-7, -2.8f}); // Mato Grosso
    Star muliphen(4, {-6.15f, -2.15f}); // Rondônia

    //randomizeColor();
    // Canis Minor
    Star procyon(1, {-7.8f, 1.2f}); // Amazonas

    /* Texto da faixa */ 
    glColor3ub(FlagColors::GREEN.r, FlagColors::GREEN.g, FlagColors::GREEN.b);
    glPushMatrix();

    const float midRadius = (arcInnerRadius + arcOuterRadius) / 2.0f; // raio médio da faixa
    // Calcular ângulos do arco visível
    auto [startAngle, endAngle] = computeArcAngles(arcCenter, midRadius, CENTER, RADIUS);

    glTranslatef(arcCenter.x, arcCenter.y, 0.0f); // Centralizar no centro do arco

    // Desenhar texto na faixa
    drawArcText("ORDEM E PROGRESSO", midRadius - 0.22f, endAngle, startAngle, 0.004f);
    glPopMatrix();

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


/** Generates a circle in Open GL frame buffer
 * @param center Posição do círculo, em relação ao centro
 * @param radius Raio do círculo
 * @param segments "Resolução"/ quantidade de segmentos do polígono gerado
 */
inline void genCircle(Vec2 center, float radius, std::size_t segments) {
    glBegin(GL_POLYGON); // Inicializa o desenho de um polígono
    float x, y;
    float offset = TAU<float> / segments;
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

/** Gera um semi-círculo em Open GL frame buffer
 * @param segmentsByArc quantidade de segmentos em cada arco (interno e externo)
 */
inline void genSemiArc(Vec2 center, float innerRadius, float outerRadius,
        float startAngle, float endAngle, std::size_t segmentsByArc) {
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

/**
 * Gera um "anel" parcial (faixa) entre dois raios (innerRadius, outerRadius)
 * de um centro arcCenter, mas somente o trecho que fica DENTRO do círculo
 * (circleCenter, circleRadius).
 */
inline void genSemiArcOverCircle(const Vec2& arcCenter, float innerRadius,
        float outerRadius, const Vec2& circleCenter, float circleRadius,
        std::size_t segments    = 64, std::size_t edgeSegments = 4)
{
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


/**
 * Desenha uma estrela regular de 5 pontas.
 * @param center Centro da estrela
 * @param outerRadius Raio externo (pontas)
 * @param innerRatio Razão do raio interno (entre 0 e 1)
 */
inline void genStar(const Vec2& center, float outerRadius, float innerRatio) {
    constexpr int numPoints = 5;
    float innerRadius = outerRadius * std::clamp(innerRatio, 0.0f, 1.0f);

    glBegin(GL_TRIANGLE_FAN);
        // Centro do fan
        glVertex2f(center.x, center.y);

        // Gera os vértices alternando externo e interno
        for (int i = 0; i <= numPoints * 2; ++i) {
            float angle = PI<float> / 2 - i * (PI<float> / numPoints); // começa pra cima
            float radius = (i % 2 == 0) ? outerRadius : innerRadius;

            float x = center.x + radius * std::cos(angle);
            float y = center.y + radius * std::sin(angle);

            glVertex2f(x, y);
        }
    glEnd();
}


/** Gera um círculo com base na qualidade.
 * @param quality Fator de suavidade ajustável. Quanto maior, mais segmentos.
 * @param min_offset Quantidade mínima de segmentos.
 * @param limit Quantidade máxima de segmentos.
 */
inline void genCircleAuto(Vec2 center, float radius, float quality = 1.5f, int min_offset = 12, int limit = 2048) {
    int segs = int(quality * radius) + min_offset; // Valor base linear + offset mínimo
    genCircle(center, radius, std::min(segs, limit));
}


inline void genCircleAuto(float x, float y, float radius, float quality = 1.5f, int min_offset = 12, int limit = 2048) {
    genCircleAuto({x, y}, radius, quality, min_offset, limit);
}


inline void randomizeColor()
{
    glColor3ub(
        (std::rand() % 256),
        (std::rand() % 256),
        (std::rand() % 256)
    );
}


/* Função para calcular o ângulo médio do arco visível */
std::pair<float, float> computeArcAngles(const Vec2& arcCenter, float radius, const Vec2& circleCenter, float circleRadius)
{
    // 1. Calcular vetor entre centros
    float dx = circleCenter.x - arcCenter.x;
    float dy = circleCenter.y - arcCenter.y;
    float d = std::sqrt(dx*dx + dy*dy);

    // 2. Calcular ângulo de direção entre centros
    float phi = std::atan2(dy, dx);

    // 3. Calcular ângulo de abertura usando lei dos cossenos
    float cosAlpha = (d * d + radius * radius - circleRadius * circleRadius) / (2.0f * d * radius);
    cosAlpha = std::clamp(cosAlpha, -1.0f, 1.0f);
    float alpha = std::acos(cosAlpha);

    return {phi - alpha, phi + alpha};
}


void drawArcText(const char* text, float radius, float startAngle, float endAngle, float scale)
{
    if (!text || *text == '\0') return;

    // Calcular ângulo total e direção
    float arcAngle = endAngle - startAngle;
    bool clockwise = (arcAngle < 0);
    float absArcAngle = std::abs(arcAngle);

    // Pré-calcular largura total do texto (unscaled)
    float totalWidth = 0;
    for (const char* c = text; *c; c++) {
        totalWidth += glutStrokeWidth(GLUT_STROKE_ROMAN, *c);
    }
    if (totalWidth <= 0) return;

    float cumulativeWidth = 0;

    for (const char* c = text; *c; c++) {
        float charWidth = glutStrokeWidth(GLUT_STROKE_ROMAN, *c);
        // Cálculo do ângulo para este caractere
        float charAngle;
        float t = (cumulativeWidth + charWidth / 2) / totalWidth;

        if (clockwise) {
            charAngle = startAngle - t * absArcAngle;
        } else {
            charAngle = startAngle + t * absArcAngle;
        }

        glPushMatrix();
        // Posicionar no ponto do arco
        float x = radius * cos(charAngle);
        float y = radius * sin(charAngle);
        glTranslatef(x, y, 0);

        // Orientação para caracteres "em pé"
        float rotation = (charAngle + PI<float> / 2) * 180.0f / PI<float>;

        // Ajuste para orientação correta
        if (clockwise) rotation += 180.0f;

        glRotatef(rotation, 0, 0, 1);

        // Centralização
        glTranslatef(-0.38f * charWidth * scale, 0.05f * charWidth * scale, 0);
        glScalef(scale, scale, scale);

        // Renderizar caractere
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        glPopMatrix();

        cumulativeWidth += charWidth;
    }
}
