
SYSFUNC void 	$_randomSeed(int32_t iSeed);
SYSFUNC void 	$_glMatrixMode(int32_t iGL_MODELVIEW);
SYSFUNC void 	$_glLoadIdentity();
SYSFUNC void 	$_glClearColor(float iRed, float iGreen, float iBlue, float iAlpha);
SYSFUNC void 	$_glClear(int32_t iGL_COLOR_BUFFER_BIT_PLUS_GL_DEPTH_BUFFER_BIT);

SYSFUNC void 	$_glBegin(int32_t iGLEnum);
SYSFUNC void 	$_glColor3f(float fX, float fY, float fZ);
SYSFUNC void 	$_glVertex2f(float fX, float fY);
SYSFUNC void 	$_glEnd();

SYSFUNC void 	$_glRotationzf(float zRot, float yRot, float fAngle);
SYSFUNC float 	$_osClock();
SYSFUNC float 	$_random();
SYSFUNC float 	$_sine(float fAngleInRadians);

// helper function to render a screen aligned Quad
void RenderQuad(float topLeft_x, float topLeft_y, float topRight_x, float topRight_y, float bottomLeft_x, float bottomLeft_y, float bottomRight_x, float bottomRight_y)
{
	$_glBegin(5); // GL_TRIANGLE_STRIP
	$_glVertex2f(topRight_x, topRight_y);
	$_glVertex2f(topLeft_x, topLeft_y);
	$_glVertex2f(bottomRight_x, bottomRight_y);
	$_glVertex2f(bottomLeft_x, bottomLeft_y);
	$_glEnd();
}

// helper function to render a screen aligned Quad with a vertical gradient
void RenderQuadVertGrad(float topLeft_x, float topLeft_y, float topRight_x, float topRight_y, float bottomLeft_x, float bottomLeft_y, float bottomRight_x, float bottomRight_y, float topGradient_r, float topGradient_g, float topGradient_b, float bottomGradient_r, float bottomGradient_g, float bottomGradient_b)
{
	$_glBegin(5); // GL_TRIANGLE_STRIP
	$_glColor3f(topGradient_r, topGradient_g, topGradient_b);
	$_glVertex2f(topRight_x, topRight_y);
	$_glVertex2f(topLeft_x, topLeft_y);
	$_glColor3f(bottomGradient_r, bottomGradient_g, bottomGradient_b);
	$_glVertex2f(bottomRight_x, bottomRight_y);
	$_glVertex2f(bottomLeft_x, bottomLeft_y);
	$_glEnd();
}

// helper function to render a screen aligned Quad with a horizontal gradient
void RenderQuadHorGrad(float topLeft_x, float topLeft_y, float topRight_x, float topRight_y, float bottomLeft_x, float bottomLeft_y, float bottomRight_x, float bottomRight_y, float leftGradient_r, float leftGradient_g, float leftGradient_b, float rightGradient_r, float rightGradient_g, float rightGradient_b)
{
	$_glBegin(5); //GL_TRIANGLE_STRIP
	$_glColor3f(rightGradient_r, rightGradient_g, rightGradient_b);
	$_glVertex2f(topRight_x, topRight_y);
	$_glColor3f(leftGradient_r, leftGradient_g, leftGradient_b);
	$_glVertex2f(topLeft_x, topLeft_y);
	$_glColor3f(rightGradient_r, rightGradient_g, rightGradient_b);
	$_glVertex2f(bottomRight_x, bottomRight_y);
	$_glColor3f(leftGradient_r, leftGradient_g, leftGradient_b);
	$_glVertex2f(bottomLeft_x, bottomLeft_y);
	$_glEnd();
}

void drawSky()
{
	// Sky
	$_glColor3f(0.25, 0.55, 0.85);
	RenderQuadVertGrad(-1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 0.05, 0.25, 0.45, 0.2, 0.45, 0.65);
}

void drawGrass()
{
	// Grass in front of the WindMill
	RenderQuadVertGrad(-1.0, -0.6, 1.0, -0.52, -1.0, -1.0, 1.0, -1.0, 0.2, 0.5, 0.2, 0.1, 0.3, 0.1);	
}

void drawWindMill()
{
	// RotorBase
	float X = -0.28;
	float Y = 0.45;
	
	// Grass behind the WindMill
	RenderQuadVertGrad(-1.0, -0.3, 1.0, -0.42, -1.0, -1.0, 1.0, -1.0, 0.2, 0.5, 0.2, 0.1, 0.35, 0.1);
	
	// bottom part
	RenderQuadHorGrad(-0.28, -0.15, -0.21, -0.15, -0.275, -0.78, -0.18, -0.75, 0.7, 0.7, 0.7, 0.8, 0.8, 0.8);
	RenderQuadHorGrad(-0.35, -0.15, -0.28, -0.15, -0.37, -0.78, -0.275, -0.75, 0.45, 0.45, 0.45, 0.7, 0.7, 0.7);

	// top part
	RenderQuadHorGrad(-0.28, 0.4, -0.23, 0.4, -0.28, -0.05, -0.215, -0.05, 0.7, 0.7, 0.7, 0.8, 0.8, 0.8);
	RenderQuadHorGrad(-0.33, 0.4, -0.28, 0.4, -0.345, -0.05, -0.28, -0.05, 0.45, 0.45, 0.45, 0.7, 0.7, 0.7);
	
	// red line
	RenderQuadHorGrad(-0.28, -0.05, -0.215, -0.05, -0.28, -0.15, -0.21, -0.15, 0.5, 0.0, 0.0, 0.6, 0.0, 0.0);
	RenderQuadHorGrad(-0.345, -0.05, -0.28, -0.05, -0.35, -0.15, -0.28, -0.15, 0.3, 0.0, 0.0, 0.5, 0.0, 0.0);
	
	// head
	RenderQuadHorGrad(X, Y - 0.1, X + 0.085, Y - 0.1, X, Y - 0.11, X + 0.055, Y - 0.11, 0.55, 0.55, 0.55, 0.6, 0.6, 0.6);
	RenderQuadHorGrad(X - 0.085, Y - 0.1, X, Y - 0.1, X - 0.055, Y - 0.11, X, Y - 0.11, 0.38, 0.38, 0.38, 0.55, 0.55, 0.55);
	RenderQuadHorGrad(X, Y + 0.1, X + 0.085, Y + 0.1, X, Y + 0.115, X + 0.055, Y + 0.115, 0.55, 0.55, 0.55, 0.6, 0.6, 0.6);
	RenderQuadHorGrad(X - 0.085, Y + 0.1, X, Y + 0.1, X - 0.055, Y + 0.115, X, Y + 0.115, 0.38, 0.38, 0.38, 0.55, 0.55, 0.55);
	RenderQuadHorGrad(X, Y + 0.1, X + 0.085, Y + 0.1, X, Y - 0.1, X + 0.085, Y - 0.1, 0.55, 0.55, 0.55, 0.6, 0.6, 0.6);
	RenderQuadHorGrad(X - 0.085, Y + 0.1, X, Y + 0.1, X - 0.085, Y - 0.1, X, Y - 0.1, 0.38, 0.38, 0.38, 0.55, 0.55, 0.55);

	$_glColor3f(0.55, 0.55, 0.55);
	
	float fTimeInSeconds = $_osClock();
	$_glRotationzf(X, Y, fTimeInSeconds * 0.6);

	// top left rotor
	RenderQuadHorGrad(-0.05, -0.004, -0.025, 0.022, -0.1884, 0.0337, -0.1553, 0.088, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65);
	RenderQuadHorGrad(-0.025, 0.022, 0.0, 0.039, -0.1553, 0.088, -0.123, 0.146, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7);
	RenderQuadHorGrad(-0.1884, 0.0337, -0.1553, 0.088, -0.53, 0.28, -0.515, 0.3, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65);
	RenderQuadHorGrad(-0.1553, 0.088, -0.123, 0.146, -0.515, 0.3, -0.5, 0.32, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7);
	RenderQuadHorGrad(-0.53, 0.28, -0.515, 0.3, -0.53, 0.3, -0.525, 0.305, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65);
	RenderQuadHorGrad(-0.515, 0.3, -0.5, 0.32, -0.525, 0.305, -0.52, 0.31, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7);

	// top right rotor
	RenderQuadHorGrad(0.515, 0.3, 0.5, 0.32, 0.525, 0.305, 0.52, 0.31, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7);
	RenderQuadHorGrad(0.53, 0.28, 0.515, 0.3, 0.53, 0.3, 0.525, 0.305, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65);
	RenderQuadHorGrad(0.1553, 0.088, 0.123, 0.146, 0.515, 0.3, 0.5, 0.32, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7);
	RenderQuadHorGrad(0.1884, 0.0337, 0.1553, 0.088, 0.53, 0.28, 0.515,0.3, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65);
	RenderQuadHorGrad(0.025, 0.022, 0.0, 0.039, 0.1553, 0.088, 0.123, 0.146, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7);
	RenderQuadHorGrad(0.05, -0.004, 0.025, 0.022, 0.1884, 0.0337, 0.1553, 0.088, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65);
	
	// bottom rotor
	RenderQuadHorGrad(0.0, -0.6, 0.02, -0.6, -0.01, -0.61, 0.01, -0.61, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7);
	RenderQuadHorGrad(-0.02, -0.6, 0.0, -0.6, -0.01, -0.61, 0.01, -0.61, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65);
	RenderQuadHorGrad(0.0, -0.18, 0.065, -0.18, 0.0, -0.6, 0.02, -0.6, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7);
	RenderQuadHorGrad(-0.065, -0.18, 0.0, -0.18, -0.02, -0.6, 0.0, -0.6, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65);
	RenderQuadHorGrad(0.0, -0.07, 0.05, -0.07, 0.0, -0.18, 0.065, -0.18, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7);
	RenderQuadHorGrad(-0.05, -0.07, 0.0, -0.07, -0.065, -0.18, 0.0, -0.18, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65);

	$_glColor3f(0.8, 0.8, 0.8);
	RenderQuad(-0.07, 0.07, 0.07, 0.07, -0.07, -0.07, 0.07, -0.07);
	$_glColor3f(0.5, 0.0, 0.0);
	RenderQuad(-0.05, 0.05, 0.05, 0.05, -0.05, -0.05, 0.05, -0.05);
	$_glColor3f(0.8, 0.8, 0.8);
	RenderQuad(-0.03, 0.03, 0.03, 0.03, -0.03, -0.03, 0.03, -0.03);
	
	$_glLoadIdentity();
}

void drawClouds()
{
	$_glColor3f(0.8, 0.8, 0.8);
	for(int8_t i = 1; i < 20; i++)
	{
		float fCurTimeInSeconds = $_osClock();
		
		float fRandX = $_random();
		float x = ( (fRandX + fCurTimeInSeconds * 0.05) % 1) * 3 - 1.5; // clouds are on the hole horizon
		
		float fRandY1 = $_random();
		float fRandY2 = $_random();
		float fCurTimeInSeconds2 = $_osClock();
		float fSine = $_sine( fRandY2 * 100.0 + 0.3 * fCurTimeInSeconds );
		float y = fRandY1 * 0.6 + 0.4 + 0.1 * fSine; // clouds are only at the top
		
		float fRandWidth = $_random();
		float width = fRandWidth * 0.25 + 0.25;
		
		float fRandHeight = $_random();
		float height = fRandHeight * 0.15 + 0.15;

		RenderQuadVertGrad(x, y, x + width, y, x, y - height, x + width, y - height, 0.9, 0.9, 0.9, 0.65, 0.65, 0.65);
	}
}

void render()
{
	$_randomSeed(0);
	$_glMatrixMode(5888); 	// GL_MODELVIEW(0x1700)
	$_glLoadIdentity();
	$_glClearColor(1.0, 0.0, 1.0, 0.0);
	$_glClear(16640);		// (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
	
	drawSky();
	
	drawClouds();
	
	drawWindMill();
	
	drawGrass();
}

void main()
{
	render();
}
