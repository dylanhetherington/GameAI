#include "GameObject.h"
#include "Texture2D.h"
#include <SDL.h>
#include <iostream>

using namespace::std;

//--------------------------------------------------------------------------------------------------

GameObject::GameObject(SDL_Renderer* renderer, GAMEOBJECT_TYPE typeOfGameObject, Vector2D startPosition, string imagePath)
{
	mRenderer = renderer;

	mTexture = new Texture2D(renderer);
	mTexture->LoadFromFile(imagePath);

	mPosition		 = startPosition;
	mRotationAngle	 = 0.0f;
	mGameObjectType  = typeOfGameObject;

	mCollisionRadius   = 0.0f;
	mCollisionRadiusSq = 0.0f;

	mAdjustedBoundingBox.push_back(Vector2D(0.0, 0.0f));
	mAdjustedBoundingBox.push_back(Vector2D(0.0f, 0.0f));
	mAdjustedBoundingBox.push_back(Vector2D(0.0f, 0.0f));
	mAdjustedBoundingBox.push_back(Vector2D(0.0f, 0.0f));

	UpdateAdjustedBoundingBox();
}

//--------------------------------------------------------------------------------------------------

GameObject::~GameObject()
{
	delete mTexture;
	mTexture = NULL;
}

//--------------------------------------------------------------------------------------------------

void GameObject::Update(float deltaTime, SDL_Event e)
{
	UpdateAdjustedBoundingBox();

	cout << "GAMEOBJECT: Must override Update." << endl;
}

//--------------------------------------------------------------------------------------------------

void GameObject::Render()
{
	mTexture->Render(mPosition, mRotationAngle);

	//Draw the collision information.
	DrawDebugCircle(GetCentralPosition(), mCollisionRadius, 0, 0, 255);
	DrawCollisionBox();
}

//--------------------------------------------------------------------------------------------------

Vector2D GameObject::GetCentralPosition()
{
	Vector2D centre = Vector2D(mPosition.x+(mTexture->GetWidth()*0.5f), mPosition.y+(mTexture->GetHeight()*0.5f));
	return centre;
}

//--------------------------------------------------------------------------------------------------

void GameObject::UpdateAdjustedBoundingBox()
{
	//Return texture size - Override for more precise collision.
	//This assumes there is only one image on the texture.
	Vector2D center = GetCentralPosition();

	double minX = center.x - (mTexture->GetWidth() / 2.0);
	double minY = center.y - (mTexture->GetHeight() / 2.0);

	double maxX = center.x + (mTexture->GetWidth() / 2.0);
	double maxY = center.y + (mTexture->GetHeight() / 2.0);

	double botLeftXRotated = (minX - center.x) * cos(mRotationAngle * M_PI / 180.0) - (minY - center.y) * sin(mRotationAngle * M_PI / 180.0);
	double botLeftYRotated = (minX - center.x) * sin(mRotationAngle * M_PI / 180.0) + (minY - center.y) * cos(mRotationAngle * M_PI / 180.0);

	double botRightXRotated = (maxX - center.x) * cos(mRotationAngle * M_PI / 180.0) - (minY - center.y) * sin(mRotationAngle * M_PI / 180.0);
	double botRightYRotated = (maxX - center.x) * sin(mRotationAngle * M_PI / 180.0) + (minY - center.y) * cos(mRotationAngle * M_PI / 180.0);

	double topRightXRotated = (maxX - center.x) * cos(mRotationAngle * M_PI / 180.0) - (maxY - center.y) * sin(mRotationAngle * M_PI / 180.0);
	double topRightYRotated = (maxX - center.x) * sin(mRotationAngle * M_PI / 180.0) + (maxY - center.y) * cos(mRotationAngle * M_PI / 180.0);

	double topLeftXRotated = (minX - center.x) * cos(mRotationAngle * M_PI / 180.0) - (maxY - center.y) * sin(mRotationAngle * M_PI / 180.0);
	double topLeftYRotated = (minX - center.x) * sin(mRotationAngle * M_PI / 180.0) + (maxY - center.y) * cos(mRotationAngle * M_PI / 180.0);

	Vector2D rotatedRect[4];
	rotatedRect[0] = Vector2D(botLeftXRotated, botLeftYRotated) + center; //Bottom Left
	rotatedRect[1] = Vector2D(botRightXRotated, botRightYRotated) + center; //Bottom Right
	rotatedRect[2] = Vector2D(topRightXRotated, topRightYRotated) + center; //Top Right
	rotatedRect[3] = Vector2D(topLeftXRotated, topLeftYRotated) + center; //Top Left

	mAdjustedBoundingBox[0] = rotatedRect[0];
	mAdjustedBoundingBox[1] = rotatedRect[1];
	mAdjustedBoundingBox[2] = rotatedRect[2];
	mAdjustedBoundingBox[3] = rotatedRect[3];
}

//--------------------------------------------------------------------------------------------------

void GameObject::DrawCollisionBox()
{
#ifdef DEBUG_LINES_VISIBLE 
	DrawDebugLine(mAdjustedBoundingBox[0], mAdjustedBoundingBox[1], 255, 0, 0);
	DrawDebugLine(mAdjustedBoundingBox[1], mAdjustedBoundingBox[2], 255, 0, 0);
	DrawDebugLine(mAdjustedBoundingBox[2], mAdjustedBoundingBox[3], 255, 0, 0);
	DrawDebugLine(mAdjustedBoundingBox[3], mAdjustedBoundingBox[0], 255, 0, 0);
#endif
}

//--------------------------------------------------------------------------------------------------

void GameObject::DrawDebugCircle(Vector2D centrePoint, double radius, int red, int green, int blue)
{
#ifdef DEBUG_LINES_VISIBLE 
	Vector2D polarVec(0.0f, radius);

	float stepSize = 0.02f;
	float _360DegAsRads = (float)DegsToRads(360.0f);

	while (polarVec.x < _360DegAsRads)
	{
		Vector2D polarAsCart(polarVec.y * cosf(polarVec.x), polarVec.y * sinf(polarVec.x));
		Vector2D drawPoint(centrePoint.x + polarAsCart.x, centrePoint.y + polarAsCart.y);

		SDL_SetRenderDrawColor(mRenderer, red, green, blue, 255);
		SDL_RenderDrawPoint(mRenderer, (int)drawPoint.x, (int)drawPoint.y);

		polarVec.x += stepSize;
	}
#endif
}

//--------------------------------------------------------------------------------------------------

void GameObject::DrawDebugLine(Vector2D startPoint, Vector2D endPoint, int red, int green, int blue)
{
#ifdef DEBUG_LINES_VISIBLE
	SDL_SetRenderDrawColor(mRenderer, red, green, blue, 255);
	SDL_RenderDrawLine(mRenderer, (int)startPoint.x, (int)startPoint.y, (int)endPoint.x, (int)endPoint.y);
#endif
}

//--------------------------------------------------------------------------------------------------
