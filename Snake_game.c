#include <stdio.h>
#include <GLFW/glfw3.h>  // GLFW library for window and input controlling.
#include <GL/gl.h>       // Core of the OpenGL for rendering the graphics.
#include <GL/glu.h>      // Utility library of OpenGL(for higher level functions). 
#include <GL/glut.h>     // GLUT library for creating the window and handle the input.
#include <stdlib.h>
#include <time.h>
#include <assert.h>

// It is define the constant value, snake length and screen size. 
#define SNAKE_LENGTH 100
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
//Arrays to store the snake directions and position.
int snake_x_direction[SNAKE_LENGTH];
int snake_y_direction[SNAKE_LENGTH];
int snake_length=3;  // Initial snake length.
int direction=0;     // For direction(Up, Right, Left, Down).
int apple_x_direction;
int apple_y_direction;
int food_count=0;


void for_snake_color()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);  // Background color(Black).
    glColor3f(1.0, 0.0, 0.0);  // Color of Snake(Red).
}


void to_provide_the_snake_move(int *x, int *y, int direction)
{
    switch(direction){
        case 0: // Up
            (*y)++;
            break;
        case 1: // Right
            (*x)++;
            break;
        case 2: // Down
            (*y)--;
            break;
        case 3: // Left
            (*x)--;
            break;
    }
    glutPostRedisplay();  // Redraw of the screen.
}


// This function each time update the body of the snake.
void update_the_position_of_the_snake()
{
    assert(snake_length>=1 && snake_length<=SNAKE_LENGTH);
    for (int i=snake_length; i>0; i--){
        snake_x_direction[i]=snake_x_direction[i-1];;
        snake_y_direction[i]=snake_y_direction[i-1];;
    }
    // assert(direction>=0 && direction<=3);
    // This part provide the movement of the snake.
    if (direction==0){
        snake_y_direction[0]++;
    }
    else if (direction==1){
        snake_x_direction[0]++;
    }
    else if (direction==2){
        snake_y_direction[0]--;
    }
    else if (direction==3){
        snake_x_direction[0]--;
    }
}


// Draw the snake on the screen.
void draw_the_snake()
{
    glColor3f(1.0, 0.0, 0.0);

    for (int i=0; i<snake_length; i++){
        float shape_x=(float)snake_x_direction[i]/20.0;
        float shape_y=(float)snake_y_direction[i]/20.0;
        glPushMatrix();  // Preserve the current transformation.
        glTranslatef(shape_x, shape_y, 0.0);  // Transfer(translate) the position of the snake.
        glBegin(GL_POLYGON);  // Draw square shape for each part of the snake.

        glVertex2f(-0.015625, 0.015625);  
        glVertex2f(0.015625, 0.015625);
        glVertex2f(0.015625, -0.015625);
        glVertex2f(-0.015625, -0.015625);
        glEnd(); 
        glPopMatrix();  // Restore the previous transformation.
    }
}

// Draw the apple on the screen.
void create_Apple()
{
    glColor3f(0.0, 1.0, 0.0); // Color of apple(Green).
    float shape_x=(float)apple_x_direction/20.0;
    float shape_y=(float)apple_y_direction/20.0;
    glPushMatrix();
    glTranslatef(shape_x, shape_y, 0.0);  // Translate the position of apple.
    glBegin(GL_POLYGON);  // Draw square for apple.
    glVertex2f(-0.015625, 0.015625);
    glVertex2f(0.015625, 0.015625);
    glVertex2f(0.015625, -0.015625);
    glVertex2f(-0.015625, -0.015625);
    glEnd();
    glPopMatrix();
}


// For randomly to put the apple in the game.
void At_Collision_Moment_Randomly_put_the_Apple(int *x, int *y)
{
    int choice=rand()%2;
    if (choice==0){
        *x=-(rand()%30);
        *y=-(rand()%21);
    }
    else{
        *x=30-(rand()%30);
        *y=21-(rand()%21+1);
    }
    if (*x==0 && *y==0){
        *x=1;   // We make to ensure that apple not at (0,0).
        *y=1;
    }
    // *y=rand()%21;  // Random y position for apple.
}


// Handling the snake growth when eat the apple.
void growth_of_the_snake()
{
    if (snake_x_direction[0]==apple_x_direction && snake_y_direction[0]==apple_y_direction){
        if (snake_length<SNAKE_LENGTH){
            snake_length++;  // We increase the snake length.
        }
        food_count++;        // Increase the count of food.
        At_Collision_Moment_Randomly_put_the_Apple(&apple_x_direction, &apple_y_direction);  // Place new apple.
    }
}


void collision_detection_case_for_window()
{
    assert(snake_length>0);
    // First case: If collision occur between snake and wall.
    if (snake_x_direction[0]<-39 || snake_x_direction[0]>=39 || snake_y_direction[0]<-22 || snake_y_direction[0]>=22){
        printf("Game over! You collision to the wall(window)\n");
        printf("Count of food: %d\n", food_count);
        exit(0);  // This part terminate the game.
    }
    
}


void collision_detection_case_for_snake()
{
    // Second case: If collision occur between snake and itself.
    for (int i=1; i<snake_length; i++){
        assert(i<SNAKE_LENGTH);
        if (snake_x_direction[0]==snake_x_direction[i] && snake_y_direction[0]==snake_y_direction[i]){
            printf("Game over! You collision to yourself\n");
            printf("Count of food: %d\n", food_count);
            exit(0);
        }
    }
}


// Sometimes this function same as the growth_of_the_snake function. But not all the time.
void to_eat_the_apple(int snake_x_direction, int snake_y_direction) 
{
    if (snake_x_direction==apple_x_direction && snake_y_direction==apple_y_direction){
        if (snake_length<SNAKE_LENGTH){
            snake_length++;
        }
        food_count++;
        growth_of_the_snake();  // When eating the food, we call this function.
        At_Collision_Moment_Randomly_put_the_Apple(&apple_x_direction, &apple_y_direction);
    }
}


// Handle snake movement for keyboard function.
void to_use_the_keyboard_for_wasd(unsigned char button, int x, int y)
{
    switch(button){
        case 'w':
            if (direction!=2){
                direction=0;
            }
            break;
        case 'd':
            if (direction!=3){
                direction=1;
            }
            break;
        case 's':
            if (direction!=0){
                direction=2;
            }
            break;
        case 'a':
            if (direction!=1){
                direction=3;
            }
            break;
    }
    glutPostRedisplay();
}


// But this handle snake movement for special keyboard function.
void snake_move_for_arrows(int key, int x, int y)
{
    switch(key){
        // These are the special functions for arrow buttons. -->, <-- etc.
        case GLUT_KEY_UP:
            // y++;
            direction=0;
            break;
        case GLUT_KEY_RIGHT:
            // x++;
            direction=1;
            break;
        case GLUT_KEY_DOWN:
            // y--;
            direction=2;
            break;
        case GLUT_KEY_LEFT:
            // x--;
            direction=3;
            break;
        
    }
}


// Creating a square.
void For_Color_of_Apple()  
{
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glColor3f(0.0, 1.0, 0.0);
}


// Update the game after a certain time interval.
void time_taken(int value)
{
    update_the_position_of_the_snake();  // Position updating.
    growth_of_the_snake();
    to_eat_the_apple(snake_x_direction[0], snake_y_direction[0]);
    collision_detection_case_for_window();
    collision_detection_case_for_snake();
    glutPostRedisplay();
    glutTimerFunc(100, time_taken, 0);  // Each time after 100ms is called spontaneously.
}


// Handle window resizing and adjust the viewport.
void to_resist_deformation_of_shape(int width, int height)
{
    glViewport(0, 0, width, height);  // It is determined viewport size.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float shape=(float)width/(float)height;  // We take the ratio in order to set the shape.
    if (width<=height){
        gluOrtho2D(-1.0, 1.0, -1.0/shape, 1.0/shape);  // Setting the othogonal projection.
    }
    else{
        gluOrtho2D(-1.0*shape, 1.0*shape, -1.0, 1.0);  // We again set the orthogonal projection but for wdth>height.
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void show_apple_and_snake()
{
    glClear(GL_COLOR_BUFFER_BIT);  // This part clear the screen. We can draw a new image.
    glLoadIdentity();  // Reset the screen position.
    draw_the_snake();
    create_Apple();
    glutSwapBuffers();  // This part show the image. For smooth animation.  
}


// This in function is considered running the game and set the functions.
int main(int C, char *argv[]){
    int x, y;  // Coordinates of the apple.
    srand(time(NULL));
    At_Collision_Moment_Randomly_put_the_Apple(&x, &y);
    glutInit(&C, argv);  // We initialize the GLUT.
    assert(snake_length<=SNAKE_LENGTH);
    assert(snake_length>=3);
    apple_x_direction=x;
    apple_y_direction=y;
    snake_x_direction[0]=10;
    snake_y_direction[0]=10;
    snake_x_direction[1]=9;
    snake_y_direction[1]=10;
    snake_x_direction[2]=8;
    snake_y_direction[2]=10;
    glutInitWindowSize(1920, 1080);  // Screen size. In this case full HD.
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);  // We set the display mode. RGB color and Double buffer.
    glutCreateWindow("Snake Game");
    For_Color_of_Apple();
    for_snake_color();
    glutDisplayFunc(show_apple_and_snake);
    glutReshapeFunc(to_resist_deformation_of_shape);  // We set the reshape function.
    glutTimerFunc(100, time_taken, 0);  // We start the timer for game updating.
    glutKeyboardFunc(to_use_the_keyboard_for_wasd);  // This function handled the keyboard function.
    glutSpecialFunc(snake_move_for_arrows);  // This function handle the special function.
    glutMainLoop();  // We start the main loop and running the game.
    return 0;
}

