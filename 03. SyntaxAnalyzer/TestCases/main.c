
// A Simple Virtual Machine 
// 		- https://www.i-programmer.info/programming/theory/3706-a-simple-virtual-machine.html

// Let’s Build A Simple Interpreter
//		- https://ruslanspivak.com/lsbasi-part1/

// A simple interpreter from scratch in Python
//		- http://jayconrod.com/posts/37/a-simple-interpreter-from-scratch-in-python--part-1-

// Ravi Programming Language’s documentation!
//		- https://the-ravi-programming-language.readthedocs.io/en/latest/index.html

// https://gamepopper.co.uk/posts/page/1/

SYSFUNC void 	$_randomSeed(int32_t iSeed);
SYSFUNC void 	$_glMatrixMode(int32_t iGL_MODELVIEW);
SYSFUNC void 	$_glLoadIdentity();
SYSFUNC void 	$_glClearColor(float iRed, float iGreen, float iBlue, float iAlpha);
SYSFUNC void 	$_glClear(int32_t iGL_COLOR_BUFFER_BIT_PLUS_GL_DEPTH_BUFFER_BIT);

SYSFUNC void 	$_glBegin(int32_t iGLEnum);
SYSFUNC void 	$_glColor3f(float fX, float fY, float fZ);
SYSFUNC void 	$_glVertex2f(float fX, float fY);
SYSFUNC void 	$_glVertex2i(int32_t iX, int32_t iY);
SYSFUNC void 	$_glEnd();

SYSFUNC void 	$_glRotationzf(float zRot, float yRot, float fAngle);
SYSFUNC float 	$_osClock_ElapsedTimeinSeconds();
SYSFUNC float 	$_osClock_DeltaTimeInMs();
SYSFUNC float 	$_random();
SYSFUNC float 	$_sine(float fAngleInRadians);

SYSFUNC int32_t	$_getScreenWidth();
SYSFUNC int32_t	$_getScreenHeight();
SYSFUNC int32_t	$_getMouseX();
SYSFUNC int32_t	$_getMouseY();
SYSFUNC	int32_t	$_isKeyPressed(int32_t iKeyCode);

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
	$_glRotationzf(X, Y, $_osClock_ElapsedTimeinSeconds() * 0.6);

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
		float x = ( ( ( $_random() + $_osClock_ElapsedTimeinSeconds() * 0.05 ) ) % 1 ) * 3 - 1.5; 						// clouds are on the hole horizon
		float y = $_random() * 0.6 + 0.4 + 0.1 * $_sine( $_random() * 100 + 0.3 * $_osClock_ElapsedTimeinSeconds() ); 	// clouds are only at the top

		float width = $_random() * 0.25 + 0.25;
		float height = $_random() * 0.15 + 0.15;

		RenderQuadVertGrad(x, y, x + width, y, x, y - height, x + width, y - height, 0.9, 0.9, 0.9, 0.65, 0.65, 0.65);
	}
}

int32_t ClipX(int32_t x)
{
	int32_t iSW = $_getScreenWidth();
	int32_t iReturnX = x;
	
	if (x < 0) 
	{
		iReturnX = 0;
	}
	if (x >= iSW) 
	{
		iReturnX = iSW;
	}
	
	return iReturnX;
}

int32_t ClipY(int32_t y)
{
	int32_t iSH = $_getScreenHeight();
	int32_t iReturnY = y;
	
	if (y < 0)
	{	
		iReturnY = 0;
	}
	if (y >= iSH)
	{
		iReturnY = iSH;
	}
	
	return iReturnY;
}


void draw(int32_t x, int32_t y, float fRed, float fGreen, float fBlue)
{
	float fX = (x * 1.0) / $_getScreenWidth();
	float fY = (($_getScreenHeight() - y) * 1.0) / $_getScreenHeight();
	
	$_glBegin(0); // GL_POINTS
		$_glColor3f(fRed, fGreen, fBlue);
		$_glVertex2f(fX, fY);
	$_glEnd();
}

void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, float fRed, float fGreen, float fBlue)
{
	float fHalfW = $_getScreenWidth() / 2.0;
	float fHalfH = $_getScreenHeight() / 2.0;

	float fX1 = (x1 - fHalfW) / fHalfW;
	float fY1 = ((fHalfH - y1) * 1.0) / fHalfH;

	float fX2 = (x2 - fHalfW) / fHalfW;
	float fY2 = ((fHalfH - y2) * 1.0) / fHalfH;
	
	$_glColor3f(fRed, fGreen, fBlue);
	$_glBegin(1); // GL_LINES
		$_glVertex2f(fX1, fY1);
		$_glVertex2f(fX2, fY2);
	$_glEnd();
}

void Fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, float fRed, float fGreen, float fBlue)
{
	int32_t xX1 = ClipX(x1);
	int32_t yY1 = ClipY(y1);
	int32_t xX2 = ClipX(x2);
	int32_t yY2 = ClipY(y2);
	
	float fHalfW = $_getScreenWidth() / 2.0;
	float fHalfH = $_getScreenHeight() / 2.0;
	
	float xLeft = (xX1 - fHalfW) / fHalfW;
	float yTop = ((fHalfH - yY1) * 1.0) / fHalfH;

	float xRight = (xX2 - fHalfW) / fHalfW;
	float yBottom = ((fHalfH - yY2) * 1.0) / fHalfH;
	
	$_glColor3f(fRed, fGreen, fBlue);
	RenderQuad(xLeft, yTop, xRight, yTop, xLeft, yBottom, xRight, yBottom);
}

void DrawMouse()
{
	int32_t iMouseX = $_getMouseX();
	int32_t iMouseY = $_getMouseY();
	
	int32_t iX1 = iMouseX - 2;
	int32_t iY1 = iMouseY - 2;
	int32_t iX2 = iMouseX + 2;
	int32_t iY2 = iMouseY + 2;
	Fill(iX1, iY1, iX2, iY2, 1.0, 0.0, 1.0);
}

struct Player
{
	int32_t m_iPosX = 0;
	int32_t m_iPosY = 0;
	int32_t m_iID = -1;
	
	inline void Player()
	{
		m_iPosX = 100;
		m_iPosY = 100;
		m_iID = -1;
	}
	
	inline void setPosition(int32_t iX, int32_t iY)
	{
		m_iPosX = iX;
		m_iPosY = iY;
	}
	
	inline int32_t getPosX()
	{
		return m_iPosX;
	}
	
	inline void setPosX(int32_t iX)
	{
		m_iPosX = iX;
	}
	
	inline int32_t getPosY()
	{
		return m_iPosY;
	}

	inline void setPosY(int32_t iY)
	{
		m_iPosY = iY;
	}
	
	inline void setID(int32_t iID)
	{
		m_iID = iID;
	}	
};

static Player* pPlayer;

void UpdatePlayer()
{
	int32_t iOff = ($_osClock_DeltaTimeInMs() * 200.0 * 0.001);
	
	if($_isKeyPressed(119) >= 1) // 'w'
	{
		pPlayer->m_iPosY = pPlayer->m_iPosY - iOff;
	}
	if($_isKeyPressed(115) >= 1) // 's'
	{
		pPlayer->m_iPosY = pPlayer->m_iPosY + iOff;
	}
	
	if($_isKeyPressed(97) >= 1) // 'a'
	{
		pPlayer->m_iPosX = pPlayer->m_iPosX - iOff;
	}
	if($_isKeyPressed(100) >= 1) // 'd'
	{
		pPlayer->m_iPosX = pPlayer->m_iPosX + iOff;
	}

	int32_t iX = pPlayer->getPosX();
	int32_t iY = pPlayer->getPosY();

	int32_t iSW = $_getScreenWidth();
	int32_t iSH = $_getScreenHeight();

	if(iX > iSW)
	{
		iX = 0;
		pPlayer->setPosX(0);
	}
	if(iX < 0)
	{
		iX = iSW;
		pPlayer->setPosX(iX);
	}
	
	if(iY > iSH)
	{
		iY = 0;
		pPlayer->setPosY(0);
	}
	if(iY < 0)
	{
		iY = iSH;
		pPlayer->setPosY(iY);
	}

	Fill(iX - 5, iY - 5, iX + 5, iY + 5, 0.0, 1.0, 0.0);
}

void render()
{	
	$_randomSeed(0);
	$_glMatrixMode(5888); 	// GL_MODELVIEW(0x1700)
	$_glLoadIdentity();
	$_glClearColor(0.0, 0.0, 0.0, 0.0);
	$_glClear(16640);		// (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
	
	drawSky();
	
	drawClouds();
	
	drawWindMill();
	
	drawGrass();
	
	DrawLine(0, 0, 50, 50, 1.0, 1.0, 1.0);
	
	DrawMouse();

	// Update Player
	if(pPlayer == 0)
	{
		pPlayer = new Player();
		pPlayer->setPosition(400, 400);
		pPlayer->setID(1);
	}

	UpdatePlayer();
}

void main()
{
	render();
}
