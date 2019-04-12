--helper function to render a screen aligned Quad
function RenderQuad(topLeft_x, topLeft_y, topRight_x, topRight_y, bottomLeft_x, bottomLeft_y, bottomRight_x, bottomRight_y)
glBegin(GL_TRIANGLE_STRIP);
glVertex2f(topRight_x, topRight_y);
glVertex2f(topLeft_x, topLeft_y);
glVertex2f(bottomRight_x, bottomRight_y);
glVertex2f(bottomLeft_x, bottomLeft_y);
glEnd();
end
--helper function to render a screen aligned Quad with a vertical gradient
function RenderQuadVertGrad(topLeft_x, topLeft_y, topRight_x, topRight_y, bottomLeft_x, bottomLeft_y, bottomRight_x, bottomRight_y, topGradient_r, topGradient_g, topGradient_b, bottomGradient_r, bottomGradient_g, bottomGradient_b)
glBegin(GL_TRIANGLE_STRIP);
glColor3f(topGradient_r, topGradient_g, topGradient_b)
glVertex2f(topRight_x, topRight_y);
glVertex2f(topLeft_x, topLeft_y);
glColor3f(bottomGradient_r, bottomGradient_g, bottomGradient_b);
glVertex2f(bottomRight_x, bottomRight_y);
glVertex2f(bottomLeft_x, bottomLeft_y);
glEnd();
end
--helper function to render a screen aligned Quad with a horizontal gradient
function RenderQuadHorGrad(topLeft_x, topLeft_y, topRight_x, topRight_y, bottomLeft_x, bottomLeft_y, bottomRight_x, bottomRight_y, leftGradient_r, leftGradient_g, leftGradient_b, rightGradient_r, rightGradient_g, rightGradient_b)
glBegin(GL_TRIANGLE_STRIP);
glColor3f(rightGradient_r, rightGradient_g, rightGradient_b)
glVertex2f(topRight_x, topRight_y);
glColor3f(leftGradient_r, leftGradient_g, leftGradient_b)
glVertex2f(topLeft_x, topLeft_y);
glColor3f(rightGradient_r, rightGradient_g, rightGradient_b)
glVertex2f(bottomRight_x, bottomRight_y);
glColor3f(leftGradient_r, leftGradient_g, leftGradient_b)
glVertex2f(bottomLeft_x, bottomLeft_y);
glEnd();
end

math.randomseed(0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glClearColor(0, 0, 0, 0);
glClear(GL_COLOR_BUFFER_BIT + GL_DEPTH_BUFFER_BIT);

--Grass
RenderQuadVertGrad(-1, -0.6, 1, -0.52, -1, -1, 1, -1, 0.2, 0.5,0.2,0.1, 0.3, 0.1);

--RotorBase
local X = -0.28
local Y = 0.45
glRotationzf(X,Y, os.clock()*0.6)
glColor3f(0.8,0.8,0.8)
RenderQuad(-0.03, 0.03, 0.03, 0.03, -0.03, -0.03, 0.03, -0.03)
glColor3f(0.5, 0, 0)
RenderQuad(-0.05, 0.05, 0.05, 0.05, -0.05, -0.05, 0.05, -0.05)
glColor3f(0.8,0.8,0.8)
RenderQuad(-0.07, 0.07, 0.07, 0.07, -0.07, -0.07, 0.07, -0.07)
--bottom rotor
RenderQuadHorGrad(-0.05, -0.07, 0, -0.07, -0.065, -0.18, 0, -0.18, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65)
RenderQuadHorGrad(0, -0.07, 0.05, -0.07, 0, -0.18, 0.065, -0.18, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7)
RenderQuadHorGrad(-0.065, -0.18, 0, -0.18, -0.02, -0.6, 0, -0.6, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65)
RenderQuadHorGrad(0, -0.18, 0.065, -0.18, 0, -0.6, 0.02, -0.6, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7)
RenderQuadHorGrad(-0.02, -0.6, 0, -0.6, -0.01, -0.61, 0.01, -0.61, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65)
RenderQuadHorGrad(0, -0.6, 0.02, -0.6, -0.01, -0.61, 0.01, -0.61, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7)
--top right rotor
RenderQuadHorGrad(0.05, -0.004, 0.025, 0.022, 0.1884, 0.0337, 0.1553, 0.088, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65)
RenderQuadHorGrad(0.025, 0.022, 0, 0.039, 0.1553, 0.088, 0.123, 0.146, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7)
RenderQuadHorGrad(0.1884, 0.0337, 0.1553, 0.088, 0.53, 0.28,0.515,0.3, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65)
RenderQuadHorGrad(0.1553, 0.088, 0.123, 0.146, 0.515, 0.3,0.5,0.32, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7)
RenderQuadHorGrad(0.53, 0.28,0.515,0.3,0.53,0.3,0.525, 0.305, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65)
RenderQuadHorGrad(0.515, 0.3,0.5,0.32,0.525,0.305,0.52, 0.31, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7)
--top left rotor
RenderQuadHorGrad(-0.05, -0.004, -0.025, 0.022, -0.1884, 0.0337, -0.1553, 0.088, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65)
RenderQuadHorGrad(-0.025, 0.022, 0, 0.039, -0.1553, 0.088, -0.123, 0.146, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7)
RenderQuadHorGrad(-0.1884, 0.0337, -0.1553, 0.088, -0.53, 0.28,-0.515,0.3, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65)
RenderQuadHorGrad(-0.1553, 0.088, -0.123, 0.146, -0.515, 0.3,-0.5,0.32, 0.65, 0.65, 0.65, 0.7, 0.7, 0.7)
RenderQuadHorGrad(-0.53, 0.28,-0.515,0.3,-0.53,0.3,-0.525, 0.305, 0.5, 0.5, 0.5, 0.65, 0.65, 0.65)
RenderQuadHorGrad(-0.515, 0.3,-0.5,0.32,-0.525,0.305,-0.52, 0.31, 0.65, 0.65 ,0.65, 0.7, 0.7, 0.7)
glLoadIdentity();

--head
glColor3f(0.55,0.55,0.55)
RenderQuadHorGrad(X-0.085, Y + 0.1, X, Y+0.1, X-0.085, Y-0.1, X, Y-0.1, 0.38, 0.38, 0.38, 0.55, 0.55, 0.55)
RenderQuadHorGrad(X, Y + 0.1, X+0.085, Y+0.1, X, Y-0.1, X+0.085, Y-0.1, 0.55, 0.55, 0.55, 0.6, 0.6, 0.6)
RenderQuadHorGrad(X-0.085, Y + 0.1, X, Y+0.1, X-0.055, Y + 0.115, X, Y+0.115, 0.38, 0.38, 0.38, 0.55, 0.55, 0.55)
RenderQuadHorGrad(X, Y + 0.1, X+0.085, Y+0.1, X, Y + 0.115, X+0.055, Y+0.115, 0.55, 0.55, 0.55, 0.6, 0.6, 0.6)
RenderQuadHorGrad(X-0.085, Y-0.1, X, Y-0.1, X-0.055, Y-0.11, X, Y-0.11, 0.38, 0.38, 0.38, 0.55, 0.55, 0.55)
RenderQuadHorGrad(X, Y-0.1, X+0.085, Y-0.1, X, Y-0.11, X+0.055, Y-0.11, 0.55, 0.55, 0.55, 0.6, 0.6, 0.6)

--red line
RenderQuadHorGrad(-0.345, -0.05, -0.28, -0.05, -0.35, -0.15, -0.28, -0.15, 0.3, 0, 0, 0.5, 0, 0)
RenderQuadHorGrad(-0.28, -0.05, -0.215, -0.05, -0.28, -0.15, -0.21, -0.15, 0.5, 0, 0, 0.6, 0, 0)

--top part
RenderQuadHorGrad(-0.33, 0.4, -0.28, 0.4, -0.345, -0.05, -0.28, -0.05, 0.45, 0.45, 0.45, 0.7, 0.7, 0.7)
RenderQuadHorGrad(-0.28, 0.4, -0.23, 0.4, -0.28, -0.05, -0.215, -0.05, 0.7, 0.7, 0.7, 0.8, 0.8, 0.8)

--bottom part
RenderQuadHorGrad(-0.35, -0.15, -0.28, -0.15, -0.37, -0.78, -0.275, -0.75, 0.45, 0.45, 0.45, 0.7, 0.7, 0.7)
RenderQuadHorGrad(-0.28, -0.15, -0.21, -0.15, -0.275, -0.78, -0.18, -0.75, 0.7, 0.7, 0.7, 0.8, 0.8, 0.8)

RenderQuadVertGrad(-1, -0.3, 1, -0.42, -1, -1, 1, -1, 0.2, 0.5, 0.2, 0.1, 0.35, 0.1);

--clouds
glColor3f(0.8, 0.8, 0.8)
for i = 1, 20 do
local x = (((math.random() + os.clock()*0.05))%1)*3-1.5 --clouds are on the hole horizon
local y = math.random()*0.6 + 0.4 + 0.1*math.sin(math.random()*100 + 0.3*os.clock()) --clouds are only at the top
local width = math.random() * 0.25 + 0.25
local height = math.random() * 0.15 + 0.15
RenderQuadVertGrad(x, y, x+width, y, x, y-height, x+width, y-height, 0.9,0.9,0.9,0.65,0.65,0.65)
end
--Sky
glColor3f(0.25, 0.55, 0.85)
RenderQuadVertGrad(-1,1,1,1,-1,-1,1,-1, 0.05, 0.25, 0.45, 0.2, 0.45, 0.65);


