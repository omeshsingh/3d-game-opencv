#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <iostream>

bool play = false;
int playcase = 1;
int level = 1;
int lives = 3;
//bool gameover = false;
//bool gameclear = false;
int dummyCount = 0;
int score = 0;

// Constants for mouse sensitivity and conversion
const float MOUSE_SENSITIVITY = 0.2f;
const float PI = 3.14159265358979323846f;

// Camera variables
float cameraAngleY = 0.0f; // Horizontal angle (yaw)
float cameraAngleX = 0.0f; // Vertical angle (pitch)

// Window dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

float theta_x = 0.0f, theta_y = 0.0f;

// Camera angles
float angleX = 0.0f, angleY = 0.0f;
int lastMouseX = WIDTH / 2, lastMouseY = HEIGHT / 2;

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
std::vector<Bullet> bullets1;

// Store multiple dummies
std::vector<Dummy> dummies;

void update(int value);

// Initialize OpenGL
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.8f, 0.9f, 1.0f); // Sky color
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
}

/*
void centerAndHideCursor() {
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    glutWarpPointer(windowWidth / 2, windowHeight / 2);
    //glutSetCursor(GLUT_CURSOR_NONE);
}
*/

// Handle mouse movement
void mouseMotion(int x, int y) {
	if(play)
	{
		float dx = x - lastMouseX;
		float dy = y - lastMouseY;

		angleX += dx * 0.1f;
		angleY += dy * 0.1f;

		lastMouseX = x;
		lastMouseY = y;
		
		/*
		int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
		int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
		
		// Calculate the window boundaries
		int windowLeft = glutGet(GLUT_WINDOW_X);
		int windowTop = glutGet(GLUT_WINDOW_Y);
		int windowRight = windowLeft + windowWidth;
		int windowBottom = windowTop + windowHeight;

		// Clamp mouse coordinates to window boundaries
		if (x < windowLeft) x = windowLeft;
		if (x > windowRight) x = windowRight;
		if (y < windowTop) y = windowTop;
		if (y > windowBottom) y = windowBottom;
		*/
		
		//centerAndHideCursor();
		glutPostRedisplay();
    }
}

// Handle mouse clicks
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        Bullet bullet;
        bullet.active = true;
        bullet.x = playerX;
        bullet.y = playerY;
        bullet.z = playerZ;
        bullet.dirX = sin(angleX * M_PI / 180.0f);
        bullet.dirY = -tan(angleY * M_PI / 180.0f);
        bullet.dirZ = -cos(angleX * M_PI / 180.0f);
        bullets.push_back(bullet);
    }
}

// Handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.5f;

    switch (key) {
		case 'w':
			if(play) {
				playerX += moveSpeed * sin(angleX * M_PI / 180.0f);
				playerY -= moveSpeed * tan(angleY * M_PI / 180.0f);
				playerZ -= moveSpeed * cos(angleX * M_PI / 180.0f);
			}
			break;
		case 's':
			if(play) {
				playerX -= moveSpeed * sin(angleX * M_PI / 180.0f);
				playerY += moveSpeed * tan(angleY * M_PI / 180.0f);
				playerZ += moveSpeed * cos(angleX * M_PI / 180.0f);
			}
			break;
		case 'a':
			if(play) {
				playerX -= moveSpeed * cos(angleX * M_PI / 180.0f);
				playerZ -= moveSpeed * sin(angleX * M_PI / 180.0f);
			}
			break;
		case 'd':
			if(play) {
				playerX += moveSpeed * cos(angleX * M_PI / 180.0f);
				playerZ += moveSpeed * sin(angleX * M_PI / 180.0f);
			}
			break;
		case ' ': // Space key
			if(play) {
				playerY += moveSpeed; // Move up
			}
			break;
		case 'z': // Shift key (or use any other key if desired)
			if(play) {
				playerY -= moveSpeed; // Move down
			}
			break;
		case '\r': //Enter key
			if((!play) && (lives > 0))
			{
				if(playcase == 2)
				{
					lives--;
				}
				
				else if(playcase == 3)
				{
					level++;
					srand(time(NULL));
					for (int i = 0; i < 10; ++i) {
						Dummy dummy;
						dummy.x = (rand() % 40) - 20;
						dummy.z = (rand() % 40) - 20;
						dummy.y = 0.0f;
						dummy.visible = true;
						dummies.push_back(dummy);
					}
				}
				
				int i = 0;
				for(auto& dummy : dummies)
				{
					dummy.visible = true;
					dummy.shootTime = time(NULL) + i + 5;
					i++;
				}
				
				if(lives > 0)
				{
					play = true;
					playcase = 0;
					dummyCount = 0;
					score = 0;
					glutTimerFunc(16, update, 0);
					bullets.clear();
					bullets1.clear();
				}
			}
			break;
    }

    glutPostRedisplay();
}

// Draw the ground
void drawGround() {
    glColor3f(0.2f, 0.7f, 0.2f); // Ground color
    glBegin(GL_QUADS);
        glVertex3f(-100.0f, -1.0f, -100.0f);
        glVertex3f(100.0f, -1.0f, -100.0f);
        glVertex3f(100.0f, -1.0f, 100.0f);
        glVertex3f(-100.0f, -1.0f, 100.0f);
    glEnd();
}

// Draw the sky
void drawSky() {
    glColor3f(0.5f, 0.8f, 0.9f); // Sky color
    glBegin(GL_QUADS);
        glVertex3f(-100.0f, 100.0f, -100.0f);
        glVertex3f(100.0f, 100.0f, -100.0f);
        glVertex3f(100.0f, 100.0f, 100.0f);
        glVertex3f(-100.0f, 100.0f, 100.0f);
    glEnd();
}

// Draw a sphere
void drawSphere(float radius, int slices, int stacks) {
    glutSolidSphere(radius, slices, stacks);
}

// Draw a cylinder
void drawCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluCylinder(quadric, baseRadius, topRadius, height, slices, stacks);
    gluDeleteQuadric(quadric);
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
    drawCylinder(0.3f, 0.3f, 1.0f, 20, 20); // Vertical cylinder for body
    glPopMatrix();

    // Arms
    glColor3f(0.8f, 0.6f, 0.4f); // Skin color
    glPushMatrix();
    glTranslatef(0.3f, 0.7f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawCylinder(0.1f, 0.1f, 0.6f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.3f, 0.7f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    drawCylinder(0.1f, 0.1f, 0.6f, 20, 20);
    glPopMatrix();

    // Legs
    glColor3f(0.2f, 0.2f, 0.2f); // Pants color
    glPushMatrix();
    glTranslatef(0.15f, 0.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.1f, 0.1f, 1.2f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.15f, 0.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.1f, 0.1f, 1.2f, 20, 20);
    glPopMatrix();
}

// Draw the crosshair
void drawCrosshair() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();


    
    if(!play)
    {
    	
    	if(playcase == 1)
    	{
			glColor3f(0.0f, 0.0f, 0.0f);
			glRasterPos2f(WIDTH/2 - 30, HEIGHT/2 + 140);  // Position of the text
			std::string gameOverStr = "INSTRUCTIONS";
			for (char c : gameOverStr) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
			}
			
			glRasterPos2f(WIDTH/2 - 70, HEIGHT/2 + 100);  // Position of the text
			gameOverStr = "1. Use 'WASD' to move around";
			for (char c : gameOverStr) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
			}
			glRasterPos2f(WIDTH/2 - 60, HEIGHT/2 + 80);  // Position of the text
			gameOverStr = "W -> Move forward";
			for (char c : gameOverStr) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
			}
			glRasterPos2f(WIDTH/2 - 60, HEIGHT/2 + 60);  // Position of the text
			gameOverStr = "S -> Move backward";
			for (char c : gameOverStr) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
			}
			glRasterPos2f(WIDTH/2 - 60, HEIGHT/2 + 40);  // Position of the text
			gameOverStr = "A -> Move left";
			for (char c : gameOverStr) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
			}
			glRasterPos2f(WIDTH/2 - 60, HEIGHT/2 + 20);  // Position of the text
			gameOverStr = "D -> Move right";
			for (char c : gameOverStr) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
			}
			
			glRasterPos2f(WIDTH/2 - 70, HEIGHT/2);  // Position of the text
			gameOverStr = "2. Use 'MOUSE' to aim";
			for (char c : gameOverStr) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
			}
			
			glRasterPos2f(WIDTH/2 - 70, HEIGHT/2 - 20);  // Position of the text
			gameOverStr = "3. Use 'MOUSE LEFT CLICK' to shoot";
			for (char c : gameOverStr) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
			}
			
			glRasterPos2f(WIDTH/2 - 50, HEIGHT/2 - 80);  // Position of the text
			gameOverStr = "PRESS ENTER TO START";
			for (char c : gameOverStr) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
			}
		}
		
    	else if(playcase == 2)
    	{
			glColor3f(0.8f, 0.0f, 0.0f);  // Red color
			if(lives > 0)
			{
				glRasterPos2f(WIDTH/2 - 30, HEIGHT/2 + 20);  // Position of the text
				std::string gameOverStr = "YOU LOST";
				for (char c : gameOverStr) {
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
				}
				
				glRasterPos2f(WIDTH/2 - 50, HEIGHT/2);
				gameOverStr = "LIVES REMAINING: ";
				std::string livesStr = std::to_string(lives-1);
				gameOverStr.append(livesStr);
				for (char c : gameOverStr) {
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
				}
				
				glRasterPos2f(WIDTH/2 - 80, HEIGHT/2 - 20);
				gameOverStr = "PRESS ENTER TO TRY AGAIN";
				for (char c : gameOverStr) {
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
				}
			}
				
			else
			{
				glRasterPos2f(WIDTH/2 - 30, HEIGHT/2 + 10);  // Position of the text
				std::string gameOverStr = "GAME OVER";
				for (char c : gameOverStr) {
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
				}
				
				glRasterPos2f(WIDTH/2 - 50, HEIGHT/2 - 10);
				gameOverStr = "LIVES REMAINING: ";
				std::string livesStr = std::to_string(lives);
				gameOverStr.append(livesStr);
				for (char c : gameOverStr) {
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
				}
			}
		}
		else if(playcase == 3)
		{
			glColor3f(0.0f, 1.0f, 1.0f);  // Red color
			glRasterPos2f(WIDTH/2 - 100, HEIGHT/2 + 10);  // Position of the text
			std::string gameOverStr = "CONGRATULATIONS, GAME CLEARED";
			for (char c : gameOverStr) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
			}
			glRasterPos2f(WIDTH/2 - 100, HEIGHT/2 - 10);  // Position of the text
			gameOverStr = "PRESS ENTER TO PLAY THE NEXT LEVEL";
			for (char c : gameOverStr) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
			}
		}
	}
	
	else
	{
		glColor3f(0.0f, 0.0f, 0.0f);  // Red color
		glRasterPos2f(10, HEIGHT - 20);  // Position of the text
		std::string scoreStr = "SCORE: ";
		std::string scoreStr1 = std::to_string(score);
		scoreStr.append(scoreStr1);
		for (char c : scoreStr) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
		}
		
		glColor3f(0.0f, 0.0f, 0.0f);  // Red color
		glRasterPos2f(WIDTH / 2, HEIGHT - 20);  // Position of the text
		std::string levelStr = "LEVEL ";
		std::string levelStr1 = std::to_string(level);
		levelStr.append(levelStr1);
		for (char c : levelStr) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
		}
		
		glColor3f(0.0f, 0.0f, 0.0f);  // Red color
		glRasterPos2f(WIDTH - 50, HEIGHT - 20);  // Position of the text
		std::string livesStr = "LIVES: ";
		std::string livesStr1 = std::to_string(lives);
		livesStr.append(livesStr1);
		for (char c : livesStr) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
		}
		
		
		glColor3f(1.0f, 1.0f, 1.0f); // Crosshair color
		glBegin(GL_LINES);
		    glVertex2f(WIDTH / 2 - 8, HEIGHT / 2);
		    glVertex2f(WIDTH / 2 + 8, HEIGHT / 2);
		    glVertex2f(WIDTH / 2, HEIGHT / 2 - 10);
		    glVertex2f(WIDTH / 2, HEIGHT / 2 + 10);
		glEnd();
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawSpaceShip() {

}

void drawSun() {
	glColor3f(1.0f, 0.0f, 0.0f); // Skin color
    glPushMatrix();
    glTranslatef(30.0f, 10.0f, 30.0f);
    drawSphere(10.0f, 20, 20);
    glPopMatrix();
}

void drawPauseScreen() {
	//glColor3f(0.5f, 0.8f, 0.9f); // Sky color
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
        glVertex3f(playerX-10.0f, playerY, playerZ-10.0f);
        glVertex3f(playerX+10.0f, playerY, playerZ-10.0f);
        glVertex3f(playerX+10.0f, playerY, playerZ+10.0f);
        glVertex3f(playerX-10.0f, playerY, playerZ+10.0f);
    glEnd();
    
    glBegin(GL_QUADS);
        glVertex3f(playerX-10.0f, playerY-10.0f, playerZ+1);
        glVertex3f(playerX+10.0f, playerY-10.0f, playerZ+1);
        glVertex3f(playerX+10.0f, playerY+10.0f, playerZ+1);
        glVertex3f(playerX-10.0f, playerY+10.0f, playerZ+1);
    glEnd();
    
    glBegin(GL_QUADS);
        glVertex3f(playerX-10.0f, playerY-10.0f, playerZ-1);
        glVertex3f(playerX+10.0f, playerY-10.0f, playerZ-1);
        glVertex3f(playerX+10.0f, playerY+10.0f, playerZ-1);
        glVertex3f(playerX-10.0f, playerY+10.0f, playerZ-1);
    glEnd();
    
    glBegin(GL_QUADS);
        glVertex3f(playerX+1, playerY-10.0f, playerZ-10.0f);
        glVertex3f(playerX+1, playerY+10.0f, playerZ-10.0f);
        glVertex3f(playerX+1, playerY+10.0f, playerZ+10.0f);
        glVertex3f(playerX+1, playerY-10.0f, playerZ+10.0f);
    glEnd();
    
    glBegin(GL_QUADS);
        glVertex3f(playerX-1, playerY-10.0f, playerZ-10.0f);
        glVertex3f(playerX-1, playerY+10.0f, playerZ-10.0f);
        glVertex3f(playerX-1, playerY+10.0f, playerZ+10.0f);
        glVertex3f(playerX-1, playerY-10.0f, playerZ+10.0f);
    glEnd();
}

// Render the scene
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set camera
    glRotatef(angleY, 1.0f, 0.0f, 0.0f);
    glRotatef(angleX, 0.0f, 1.0f, 0.0f);
    glTranslatef(-playerX, -playerY, -playerZ);
    
    //drawSpaceShip();

	if(!play)
		drawPauseScreen();

    // Draw sky
    //drawSky();

    // Draw ground
    drawGround();
    
    //drawSun();

    // Render dummies
    for (auto& dummy : dummies) {
        if (dummy.visible) {
            glPushMatrix();
            glTranslatef(dummy.x, dummy.y, dummy.z);
            drawHumanoidDummy();
            glPopMatrix();
        }
    }

    // Render bullets
    for (auto& bullet : bullets) {
        if (bullet.active) {
            glPushMatrix();
            glTranslatef(bullet.x, bullet.y, bullet.z);
            glColor3f(1.0f, 1.0f, 0.0f);
            glutSolidSphere(0.1f, 20, 20);
            glPopMatrix();
        }
    }
    
    // Render bullets1
    if(play) {
		for (auto& bullet : bullets1) {
		    if (bullet.active) {
		        glPushMatrix();
		        glTranslatef(bullet.x, bullet.y, bullet.z);
		        glColor3f(1.0f, 1.0f, 0.0f);
		        glutSolidSphere(0.1f, 20, 20);
		        glPopMatrix();
		    }
		}
	}

    // Draw crosshair
    drawCrosshair();

    glutSwapBuffers();
}

void update(int value) {
	if(play)
	{
		// Update bullets
		for (auto& bullet : bullets) {
		    if (bullet.active) {
		        bullet.x += bullet.dirX * 0.8f;
		        bullet.y += bullet.dirY * 0.8f;
		        bullet.z += bullet.dirZ * 0.8f;
		        if (bullet.z < -100.0f || bullet.z > 100.0f || bullet.x < -100.0f || bullet.x > 100.0f || bullet.y < -50.0f || bullet.y > 50.0f) {
		            bullet.active = false;
		        }

		        // Check for collision with dummies
		        for (auto& dummy : dummies) {
		            if (dummy.visible) {
		                // Define hit thresholds
		                float headRadius = 0.3f;
		                float bodyRadius = 0.2f;
		                float armRadius = 0.1f;
		                float legRadius = 0.15f;

		                float headHeight = 1.3f;
		                float bodyHeight = 1.0f;
		                float armHeight = 0.1f;
		                float legHeight = 0.1f;

		                /*
		                // Calculate distances
		                float headDistance = sqrt(pow(bullet.x - dummy.x, 2) + pow(bullet.y - (dummy.y + headHeight), 2) + pow(bullet.z - dummy.z, 2));
		                float bodyDistance = sqrt(pow(bullet.x - dummy.x, 2) + pow(bullet.y - (dummy.y + bodyHeight / 2), 2) + pow(bullet.z - dummy.z, 2));

		                float leftArmDistance = sqrt(pow(bullet.x - (dummy.x - 0.4f), 2) + pow(bullet.y - (dummy.y + armHeight), 2) + pow(bullet.z - dummy.z, 2));
		                float rightArmDistance = sqrt(pow(bullet.x - (dummy.x + 0.4f), 2) + pow(bullet.y - (dummy.y + armHeight), 2) + pow(bullet.z - dummy.z, 2));

		                float leftLegDistance = sqrt(pow(bullet.x - (dummy.x - 0.2f), 2) + pow(bullet.y - dummy.y, 2) + pow(bullet.z - dummy.z, 2));
		                float rightLegDistance = sqrt(pow(bullet.x - (dummy.x + 0.2f), 2) + pow(bullet.y - dummy.y, 2) + pow(bullet.z - dummy.z, 2));

		                // Check collisions
		                if (headDistance < headRadius || bodyDistance < bodyRadius || leftArmDistance < armRadius || rightArmDistance < armRadius || leftLegDistance < legRadius || rightLegDistance < legRadius) {
		                    dummy.visible = false;
		                    dummy.disappearTime = time(NULL) + 5; // Disappear for 5 seconds
		                }*/

		                bool body_collision_check = ((bullet.y < 1.6) && (bullet.y > -1.0) && (abs(bullet.x - dummy.x) < 0.3) && (abs(bullet.z - dummy.z) < 0.3)) ? 1 : 0;
		                bool arms_collision_check = ((bullet.y > 0.7) && (bullet.y < 0.8) && (abs(bullet.x - dummy.x) < 0.9) && (abs(bullet.z - dummy.z) < 0.3)) ? 1 : 0;

		                if(body_collision_check || arms_collision_check)
		                {
		                	dummy.visible = false;
		                	score += 5;
		                	dummyCount++;
		                	if(dummyCount == 10)
		                	{
		                		play = false;
		                		playcase = 3;
		                    //dummy.disappearTime = time(NULL) + 10;
		                    }
		                }
		                
		            }
		        }
		    }
		}
		
		for (auto& bullet : bullets1) {
		    if (bullet.active) {
		        bullet.x += bullet.dirX * 0.2f;
		        bullet.y += bullet.dirY * 0.2f;
		        bullet.z += bullet.dirZ * 0.2f;
		        if (bullet.z < -100.0f || bullet.z > 100.0f || bullet.x < -100.0f || bullet.x > 100.0f || bullet.y < -50.0f || bullet.y > 50.0f) {
		            bullet.active = false;
		        }
		        
		        bool player_collision_check = ((abs(bullet.x - playerX) < 1.0) && (abs(bullet.y - playerY) < 1.0) && (abs(bullet.z - playerZ) < 1.0)) ? 1 : 0;
		        if(player_collision_check)
		        {
		        	play = false;
		        	playcase = 2;
		        	if(lives == 1)
		        		lives = 0;
		        }
			}
		}

		// Check and update visibility of dummies
		/*
		time_t currentTime = time(NULL);
		for (auto& dummy : dummies) {
		    if (!dummy.visible && currentTime >= dummy.disappearTime) {
		        dummy.visible = true;
		    }
		}
		*/
		
		time_t currentTime = time(NULL);
		for (auto& dummy : dummies) {
			if (dummy.visible && currentTime >= dummy.shootTime) {
				//dummy.visible = true;
				//std::cout<<"check"<<std::endl;
				Bullet bullet;
				bullet.active = true;
				bullet.x = dummy.x;
				bullet.y = dummy.y;
				bullet.z = dummy.z;
				
				//set the bullets angle so that it shoots towards the player:
				
				// Calculate the direction vector from v1 to v2
				GLfloat direction_vector[3] = {playerX - dummy.x, playerY - dummy.y, playerZ - dummy.z};
				
				// Normalize the direction vector
				GLfloat length = sqrt(direction_vector[0] * direction_vector[0] + direction_vector[1] * direction_vector[1] + direction_vector[2] * direction_vector[2]);
				GLfloat direction_vector_normalized[3] = {direction_vector[0] / length, direction_vector[1] / length, direction_vector[2] / length};

				// Calculate y angle (theta_y)
				theta_y = atan2(direction_vector_normalized[0], direction_vector_normalized[2]);
				
				// Calculate x angle (theta_x)
				GLfloat xy_projection_length = sqrt(direction_vector_normalized[0] * direction_vector_normalized[0] + direction_vector_normalized[2] * direction_vector_normalized[2]);
						                            
				theta_x = atan2(direction_vector_normalized[1], xy_projection_length);
				
				bullet.dirX = sin(theta_y);
				bullet.dirY = tan(theta_x);
				//bullet.dirY = direction_vector[1] / xy_projection_length;
				bullet.dirZ = cos(theta_y);
				bullets1.push_back(bullet);
				dummy.shootTime = time(NULL) + 10;
			}
		}

		glutPostRedisplay();
		glutTimerFunc(16, update, 0);
    }
}

// Handle window resize
void resize(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("3D Shooting Game");

    initOpenGL();

    glutDisplayFunc(renderScene);
    glutReshapeFunc(resize);
    glutPassiveMotionFunc(mouseMotion);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}
