#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <iostream>

bool gameover = false;
bool gameclear = false;
int score = 0;
bool showStartPage = true; // Flag to show start page
int gameState = 0; // Game state variable

// Constants for mouse sensitivity and conversion
const float MOUSE_SENSITIVITY = 0.2f;
const float PI = 3.14159265358979323846f;

// Camera variables
float angleX = 0.0f, angleY = 0.0f;

// Window dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

// Player position
float playerX = 0.0f, playerY = 0.0f, playerZ = 20.0f;

// Bullet properties
struct Bullet {
    float x, y, z;
    float dirX, dirY, dirZ;
    bool active;
};

// Dummy properties
struct Dummy {
    float x, y, z;
    bool visible;
    time_t shootTime;
};

// Store multiple bullets
std::vector<Bullet> bullets;

// Store multiple dummies
std::vector<Dummy> dummies;

// Initialize OpenGL
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.8f, 0.9f, 1.0f); // Sky color

    // Create multiple dummies
    srand(time(NULL));
    for (int i = 0; i < 10; ++i) {
        Dummy dummy;
        dummy.x = (rand() % 40) - 20;
        dummy.z = (rand() % 40) - 20;
        dummy.y = 0.0f;
        dummy.visible = true;
        dummy.shootTime = time(NULL) + i + 5;
        dummies.push_back(dummy);
    }

    showStartPage = true; // Initialize start page display
}

// Handle mouse movement
void mouseMotion(int x, int y) {
    if (gameState == 1 && !gameover && !gameclear) {
        float dx = x - WIDTH / 2;
        float dy = y - HEIGHT / 2;

        angleX += dx * MOUSE_SENSITIVITY;
        angleY += dy * MOUSE_SENSITIVITY;

        if (angleY > 89.0f) angleY = 89.0f;
        if (angleY < -89.0f) angleY = -89.0f;

        glutWarpPointer(WIDTH / 2, HEIGHT / 2); // Center mouse
        glutPostRedisplay();
    }
}

// Handle mouse clicks
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && gameState == 1 && !gameover && !gameclear) {
        Bullet bullet;
        bullet.active = true;
        bullet.x = playerX;
        bullet.y = playerY;
        bullet.z = playerZ;
        bullet.dirX = sin(angleX * PI / 180.0f);
        bullet.dirY = -tan(angleY * PI / 180.0f);
        bullet.dirZ = -cos(angleX * PI / 180.0f);
        bullets.push_back(bullet);
    }
}

// Handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.5f;

    switch (key) {
        case 'w':
            if (gameState == 1 && !gameover && !gameclear) {
                playerX += moveSpeed * sin(angleX * PI / 180.0f);
                playerZ += moveSpeed * cos(angleX * PI / 180.0f);
            }
            break;
        case 's':
            if (gameState == 1 && !gameover && !gameclear) {
                playerX -= moveSpeed * sin(angleX * PI / 180.0f);
                playerZ -= moveSpeed * cos(angleX * PI / 180.0f);
            }
            break;
        case 'a':
            if (gameState == 1 && !gameover && !gameclear) {
                playerX += moveSpeed * cos(angleX * PI / 180.0f);
                playerZ -= moveSpeed * sin(angleX * PI / 180.0f);
            }
            break;
        case 'd':
            if (gameState == 1 && !gameover && !gameclear) {
                playerX -= moveSpeed * cos(angleX * PI / 180.0f);
                playerZ += moveSpeed * sin(angleX * PI / 180.0f);
            }
            break;
        case ' ': // Space key
            if (gameState == 1 && !gameover && !gameclear) {
                playerY += moveSpeed; // Move up
            }
            break;
        case 'z': // Shift key (or use any other key if desired)
            if (gameState == 1 && !gameover && !gameclear) {
                playerY -= moveSpeed; // Move down
            }
            break;
        case '\r': // Enter key
            if (gameover || gameclear) {
                // Reset game state
                gameState = 0;
                gameover = false;
                gameclear = false;
                score = 0;
                bullets.clear();
                dummies.clear();
                for (int i = 0; i < 10; ++i) {
                    Dummy dummy;
                    dummy.x = (rand() % 40) - 20;
                    dummy.z = (rand() % 40) - 20;
                    dummy.y = 0.0f;
                    dummy.visible = true;
                    dummy.shootTime = time(NULL) + i + 5;
                    dummies.push_back(dummy);
                }
            }
            break;
        case '1': // Start game
            if (gameState == 0) {
                gameState = 1;
                showStartPage = false;
            }
            break;
    }

    glutPostRedisplay();
}

// Draw a sphere
void drawSphere(float radius, int slices, int stacks) {
    glutSolidSphere(radius, slices, stacks);
}

// Draw a humanoid dummy
void drawHumanoidDummy() {
    // Head
    glColor3f(0.8f, 0.6f, 0.4f); // Skin color
    glPushMatrix();
    glTranslatef(0.0f, 1.3f, 0.0f);
    drawSphere(0.3f, 20, 20);
    glPopMatrix();

    // Body
    glColor3f(0.4f, 0.4f, 0.6f); // Shirt color
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotate to make vertical
    drawSphere(0.3f, 20, 20); // Vertical cylinder for body
    glPopMatrix();

    // Arms
    glColor3f(0.8f, 0.6f, 0.4f); // Skin color
    glPushMatrix();
    glTranslatef(0.3f, 0.7f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawSphere(0.1f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.3f, 0.7f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawSphere(0.1f, 20, 20);
    glPopMatrix();

    // Legs
    glColor3f(0.4f, 0.4f, 0.6f); // Pants color
    glPushMatrix();
    glTranslatef(0.15f, 0.2f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotate to make vertical
    drawSphere(0.15f, 20, 20); // Vertical cylinder for legs
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.15f, 0.2f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotate to make vertical
    drawSphere(0.15f, 20, 20); // Vertical cylinder for legs
    glPopMatrix();
}

// Draw crosshair
void drawCrosshair() {
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, HEIGHT, 0);

    // Draw crosshair lines
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    glBegin(GL_LINES);
    glVertex2i(WIDTH / 2 - 10, HEIGHT / 2);
    glVertex2i(WIDTH / 2 + 10, HEIGHT / 2);
    glVertex2i(WIDTH / 2, HEIGHT / 2 - 10);
    glVertex2i(WIDTH / 2, HEIGHT / 2 + 10);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// Display callback



void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set camera position and orientation
    gluLookAt(playerX, playerY, playerZ,
              playerX + sin(angleX * PI / 180.0f), playerY - tan(angleY * PI / 180.0f), playerZ - cos(angleX * PI / 180.0f),
              0.0f, 1.0f, 0.0f);

    // Draw dummies
    for (size_t i = 0; i < dummies.size(); ++i) {
        if (dummies[i].visible) {
            glPushMatrix();
            glTranslatef(dummies[i].x, dummies[i].y, dummies[i].z);
            drawHumanoidDummy();
            glPopMatrix();
        }
    }

    // Draw bullets
    for (size_t i = 0; i < bullets.size(); ++i) {
        if (bullets[i].active) {
            glPushMatrix();
            glTranslatef(bullets[i].x, bullets[i].y, bullets[i].z);
            glColor3f(1.0f, 0.0f, 0.0f); // Red color for bullets
            drawSphere(0.05f, 10, 10); // Bullet sphere
            glPopMatrix();
        }
    }

    // Draw score
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    glRasterPos2i(10, 10); // Position at top-left corner
    std::string scoreStr = "Score: " + std::to_string(score);
    for (char const &c : scoreStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Draw start page
    if (showStartPage) {
        glColor3f(1.0f, 1.0f, 1.0f); // White color
        glRasterPos2i(WIDTH / 2 - 100, HEIGHT / 2); // Position at center
        std::string message = "Press 1 to start the game.";
        for (char const &c : message) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    drawCrosshair(); // Draw crosshair

    glutSwapBuffers();
}


// Timer function for updating game state
void update(int value) {
    if (gameState == 1 && !gameover && !gameclear) {
        // Update dummy positions
        for (size_t i = 0; i < dummies.size(); ++i) {
            if (dummies[i].visible) {
                // Move towards player
                float moveSpeed = 0.05f;
                float dx = playerX - dummies[i].x;
                float dz = playerZ - dummies[i].z;
                float dist = sqrt(dx * dx + dz * dz);
                if (dist > 0.1f) {
                    dummies[i].x += moveSpeed * dx / dist;
                    dummies[i].z += moveSpeed * dz / dist;
                }

                // Check collision with bullets
                for (size_t j = 0; j < bullets.size(); ++j) {
                    if (bullets[j].active) {
                        float bx = bullets[j].x - dummies[i].x;
                        float by = bullets[j].y - dummies[i].y;
                        float bz = bullets[j].z - dummies[i].z;
                        float dist = sqrt(bx * bx + by * by + bz * bz);
                        if (dist < 0.3f) {
                            dummies[i].visible = false;
                            bullets[j].active = false;
                            score += 10;
                        }
                    }
                }

                // Check collision with player
                float px = playerX - dummies[i].x;
                float py = playerY - dummies[i].y;
                float pz = playerZ - dummies[i].z;
                float distPlayer = sqrt(px * px + py * py + pz * pz);
                if (distPlayer < 0.5f) {
                    gameover = true;
                }
            }

            // Check if dummies shoot bullets
            if (dummies[i].shootTime <= time(NULL)) {
                Bullet bullet;
                bullet.active = true;
                bullet.x = dummies[i].x;
                bullet.y = dummies[i].y;
                bullet.z = dummies[i].z;
                bullet.dirX = (playerX - dummies[i].x) / 10.0f;
                bullet.dirY = (playerY - dummies[i].y) / 10.0f;
                bullet.dirZ = (playerZ - dummies[i].z) / 10.0f;
                bullets.push_back(bullet);
                dummies[i].shootTime = time(NULL) + 3; // Shoot every 3 seconds
            }
        }

        glutPostRedisplay();
    }

    // Call update function again after 16ms (approx. 60 fps)
    glutTimerFunc(16, update, 0);
}

// Reshape function
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Space Shooter Game");
    initOpenGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mouseMotion);
    glutSetCursor(GLUT_CURSOR_NONE); // Hide mouse cursor
    glutMainLoop();
    return 0;
}

