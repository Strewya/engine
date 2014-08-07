
gState = {};

gState.moveCamRight = false;
gState.moveCamLeft = false;
gState.moveCamUp = false;
gState.moveCamDown = false;
gState.moveCamFwd = false;
gState.moveCamBack = false;
gState.resetCam = false;
gState.rotateCam = false;
gState.spawnApple = false;
gState.mx = 0;
gState.my = 0;
gState.rmx = 0;
gState.rmy = 0;
gState.impulseStrength = 9.5;
gState.yVel = 0;
gState.yAcc = 0;
gState.maxJumpsAvailable = 1;
gState.jumpsAvailable = gState.maxJumpsAvailable;
gState.gravity = -31;
gState.minY = -2.15;
gState.velocity = 0;
gState.maxVelocity = 5;
gState.acceleration = 1;
gState.spawnCount = 5;
gState.eatenApples = 0;
gState.targetApples = 10;
gState.gameOver = false;
gState.bestTime = 0;
gState.bboxColor = Core.Color(0,0,0);
gState.drawPositions = false;
gState.drawCollisionRect = false;

gState.treeHS = Core.Vec2(0.4, 2);
gState.treeCol = Core.Color(127/255, 63/255, 63/255);
gState.treePos = Core.Vec2(3,-0.5);
gState.treeTopHS = Core.Vec2(2, 1);
gState.treeTopCol = Core.Color(58/255, 1, 75/255);
gState.treeTopPos = Core.Vec2(3,1.2);

function toggleDrawPositions()
	gState.drawPositions = not gState.drawPositions;
end;

function toggleDrawCollision()
	gState.drawCollisionRect = not gState.drawCollisionRect;
end;