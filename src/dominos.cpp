
#include "cs296_base.hpp"
#include "render.hpp"

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include "GL/freeglut.h"
#endif

#include <cstring>
using namespace std;

#include "dominos.hpp"

namespace cs296
{
  /**  The is the constructor 
   * This is the documentation block for the constructor.
   */ 
  
  dominos_t::dominos_t()
  {
	/*!
	* convert 1 degree into radians, which is useful in future.
	*/	
	  float DegToRad = 0.0174532925;
	/*****************************************************************//**
	* Bodies are built using the following steps: 
	* 1. Define a body with position, damping, etc. 
	* 2. Use the world object to create the body. 
	* 3. Define fixtures with a shape, friction, density, etc. 
	* 4. Create fixtures on the body. 
	*********************************************************************/	
	  // main ground 
    {
	/*! Define ground(static, by default) with position at origin(0,0). */
      b2BodyDef bd; 
      bd.position.Set(0.0f, 0.0f);
	/*! create the ground in the world. */
      b2Body* b1 = m_world->CreateBody(&bd); 
	/*! Define fixtures of ground. */
	/*! shape of ground is precisely a horizontal edge joining (-90,0) and (90,0). */  
      b2EdgeShape shape; 
      shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f));
	/*! Create fixtures on the body.*/
      b1->CreateFixture(&shape, 0.0f);
	/*! second argument in the above command represents the density of the body, 
	* which is zero by default for a static body
	*/
    }
	{  
/////////////////// main block
		{
		  //b2PolygonShape shape;
		 // shape.SetAsBox(15.0f, 7.5f); // rectangle of 16x10 units
		/*! define the main body of bulldozer with its position at (0,10.5) and set it as dynamic body */
		b2BodyDef main_bd;
		main_bd.type = b2_dynamicBody;
		main_bd.position.Set(0.0f , 10.5f); 
		/*! create the main body in the world */
		main_body = m_world->CreateBody(&main_bd);
		/*! create the shape of main body */
		/*! shape of the main body is a pentagon with vertices (15,6), (15,-7.5), (-15,-7.5), (-15,0) 
		* and (-4,6) in the given order.
		*/
		/*! The above mentioned coordinates are with respect to the position of the body. */
		b2PolygonShape main_s;
	      	b2Vec2 vertices[5];
		  vertices[0].Set(15,6);
		  vertices[4].Set(15,-7.5);
		  vertices[3].Set(-15,-7.5);
		  vertices[2].Set(-15,0);
		  vertices[1].Set(-4,6);
		  main_s.Set(vertices, 5);	
		/*! Define fixtures with friction, density and above mentioned shape */
		/*! relative density = 20 */
		/*! coefficient of friction = 0.1 */
		  b2FixtureDef main_fd;
		  main_fd.shape = &main_s;
		  main_fd.density = 20.0f;
		  main_fd.friction = 0.1f;
		/*! create fixtures on the body */
		  main_body->CreateFixture(&main_fd);
		  
		//  body->SetLinearVelocity(b2Vec2( 3.0f, 0)); // giving linear velocity to the main body
		 
/////////////////////The Bigger tyre
		/*! Define a body for the bigger tyre, rear tyre */
		  b2BodyDef ballbd;
		/*! Change the body type to dynamic */
		  ballbd.type = b2_dynamicBody;
		/*! set the position of the tyre at (7,5) with respect to origin of the world */
		  ballbd.position.Set(7.0f, 5.0f);
		/*! Create the body in the world */
		  b2Body* sbody = m_world->CreateBody(&ballbd);
		
		/*! Describe the shape of the tyre to be circle of radius 5, 
		* which is also equal to the height of center of the circle from the ground 
		*/
		  b2CircleShape circle;
		  circle.m_radius = 5.0;
		/*! Define the fixtures with above shape and with */
		/*! relative density = 5.0f */
		/*! coefficient of friction = 0.1f */
		/*! coefficient of restitution = 0 (perfectly inelastic collision) */		
		  b2FixtureDef ballfd;
		  ballfd.shape = &circle;
		  ballfd.density = 5.0f;
		  ballfd.friction = 0.1f;
		  ballfd.restitution = 0.0f;
		/*! Create fixtures on the body */
		  sbody->CreateFixture(&ballfd);
		  
		/*! Now create a joint between the main body of the bulldozer and the rear tyre */
		/*! Define the joint, its a revolute joint */
		  b2RevoluteJointDef jointDef;
		/*! Two bodies(BodyA, BodyB) between which joint is to be made are are assigned 
		* accordingly(main body, rear tyre)
		*/
		  jointDef.bodyA = main_body;
		  jointDef.bodyB = sbody;
		/*! the local anchor point relative to main body's origin(0,10.5) is set to (7,-5.5) */
		  jointDef.localAnchorA.Set(7,-5.5);
		/*! the local anchor point relative to rear tyre's origin(7,5) is set to (0,0) */ 
		  jointDef.localAnchorB.Set(0,0);
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef.collideConnected = false;//doubt: is it required??
		/*! motor is enabled since it should be alowed to rotate */
		  jointDef.enableMotor = true;
		/*! Maximum motor torque used to achieve the desired motor speed is set to 500N-m */ 
		  jointDef.maxMotorTorque = 500;
		/*! Create the joint in the world */ 
		  m_world->CreateJoint(&jointDef);
		  
		  
///////////////////// small tyre
		/*! Define a body for the smaller tyre, front tyre */
		  b2BodyDef ballbd1;
		/*! Change the body type to dynamic */
		  ballbd1.type = b2_dynamicBody;
		/*! set the position of the tyre at (-7,3) with respect to origin of the world */
		  ballbd1.position.Set(-7.0f, 3.0f);
		/*! Create the body in the world */
		  b2Body* sbody1 = m_world->CreateBody(&ballbd1);

		/*! Describe the shape of the tyre to be circle of radius 3, 
		* which is also equal to the height of center of the circle from the ground 
		*/
		  b2CircleShape circle1;
		  circle1.m_radius = 3.0;
		/*! Define the fixtures with above shape and with */
		/*! relative density = 5.0f */
		/*! coefficient of friction = 0.1f */
		/*! coefficient of restitution = 0 (perfectly inelastic collision) */
		  b2FixtureDef ballfd1;
		  ballfd1.shape = &circle1;
		  ballfd1.density = 5.0f;
		  ballfd1.friction = 0.1f;
		  ballfd1.restitution = 0.0f;
		/*! Create fixtures on the body */
		  sbody1->CreateFixture(&ballfd1);
		/*! Now create a joint between the main body of the bulldozer and the front tyre */
		/*! Define the joint, its a revolute joint */ 
		  b2RevoluteJointDef jointDef1;
		/*! Two bodies(BodyA, BodyB) between which joint is to be made are are assigned 
		* accordingly(main body, front tyre)
		*/
		  jointDef1.bodyA = main_body;
		  jointDef1.bodyB = sbody1;
		/*! the local anchor point relative to main body's origin(0,10.5) is set to (-7,-7.5) */
		  jointDef1.localAnchorA.Set(-7,-7.5);
		/*! the local anchor point relative to front tyre's origin(-7,3) is set to (0,0) */ 
		  jointDef1.localAnchorB.Set(0,0);
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef1.collideConnected = false;//doubt: is it required??
		/*! motor is enabled since it should be alowed to rotate */
		  jointDef1.enableMotor = true;
		/*! Maximum motor torque used to achieve the desired motor speed is set to 500N-m */
		  jointDef1.maxMotorTorque = 500;
		/*! Create the joint in the world */ 
		  m_world->CreateJoint(&jointDef1);
		  
////////////////////left arm
		/*! Define the first part of front arm of the bulldozer */
		  b2BodyDef leftarm_bd;
		/*! set its position at (-11.5,16.5) */ 
		  leftarm_bd.position.Set(-11.5f, 16.5f);
		/*! set its body type to dynamicbody */
		  leftarm_bd.type = b2_dynamicBody;
		/*! create the first part of front arm in the world */
		  leftarm_b = m_world->CreateBody(&leftarm_bd);
		/*! Assign the shape of first part of the front arm as a rectangle of length 15m and breadth 1m */
		  b2PolygonShape leftarm_s;
		  leftarm_s.SetAsBox(7.5f, 0.5f);
		/*! Define the fixtures of first part of the front arm with the above mentioned shape and with */
		/*! Relative density = 1 */ 
		  b2FixtureDef *leftarm_fd = new b2FixtureDef;
		  leftarm_fd->density = 1.0f;
		  leftarm_fd->shape = new b2PolygonShape;
		  leftarm_fd->shape = &leftarm_s;
		/*! Create above mentioned fixtures to first part of the front arm body */
		  leftarm_b->CreateFixture(leftarm_fd);
		  
		/*! Create a joint, Revolute joint, between the main body and first part of the front arm */
		  b2RevoluteJointDef jointDef2;
		/*! The two bodies(BodyA, BodyB) between which joint is made are first part of the front arm and main body respectively */
		  jointDef2.bodyA = leftarm_b;
		  jointDef2.bodyB = main_body;
		/*! the local anchor point relative to first part of the front arm's origin(-11.5,16.5) is set to (7.5,0) */
		  jointDef2.localAnchorA.Set(7.5,0);
		/*! the local anchor point relative to main body's origin(0,10.5) is set to (-4,6) */
		  jointDef2.localAnchorB.Set(-4.0f,6.0f);
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef2.collideConnected = false;
		/*! enable limit is set to true in order to limit the rotation */
		  jointDef2.enableLimit = true;
		/*! The lower limit is set to -60 degrees, which is converted to radians using DegToRad variable defined earlier */
		  jointDef2.lowerAngle = -60*DegToRad ;
		//  jointDef2.upperAngle = 0*DegToRad ;
		/*! motor is enabled since it should be alowed to rotate */
		  jointDef2.enableMotor = true;
		/*! Maximum motor torque used to achieve the desired motor speed is set to 8000N-m */
		  jointDef2.maxMotorTorque = 8000;
        //  jointDef2.motorSpeed = 0;//360 * DegToRad;
		/*! Create the above mentioned joint in the world */ 
		  m_world->CreateJoint(&jointDef2);
		  
		  
		  //leftarm_b->ApplyLinearImpulse( b2Vec2(-5,1000), leftarm_b->GetWorldCenter() , true );
		  
//////////////////left arm sub
		/*! Define the second part of front arm of the bulldozer */
		  b2BodyDef leftarmsub_bd;
		/*! set its position at (-23,16.5) */ 
		  leftarmsub_bd.position.Set(-23.0f, 16.5f);
		/*! set its body type to dynamicbody */
		  leftarmsub_bd.type = b2_dynamicBody;
		/*! create the second part of front arm in the world */
		  b2Body* leftarmsub_b = m_world->CreateBody(&leftarmsub_bd);
		/*! Assign the shape of second part of the front arm as a rectangle of length 8m and breadth 1m */
		  b2PolygonShape leftarmsub_s;
		  leftarmsub_s.SetAsBox(4.0f, 0.5f);
		/*! Define the fixtures of second part of the front arm with the above mentioned shape and with */
		/*! Relative density = 1 */ 
		  b2FixtureDef *leftarmsub_fd = new b2FixtureDef;
		  leftarmsub_fd->density = 1.0f;
		  leftarmsub_fd->shape = new b2PolygonShape;
		  leftarmsub_fd->shape = &leftarmsub_s;
		/*! Create above mentioned fixtures to second part of the front arm body */
		  leftarmsub_b->CreateFixture(leftarmsub_fd);
		
		/*! Create a joint, Revolute joint, between first part and second part of the front arm */
		  b2RevoluteJointDef jointDef3;
		/*! The two bodies(BodyA, BodyB) between which joint is made are first part and second part 
		* of the front arm respectively 
		*/
		  jointDef3.bodyA = leftarm_b;
		  jointDef3.bodyB = leftarmsub_b;
		/*! the local anchor point relative to first part of the front arm's origin(-11.5,16.5) is set to (-7.5,0) */
		  jointDef3.localAnchorA.Set(-7.5,0);
		/*! the local anchor point relative to second part of the front arm's origin(-23,16.5) is set to (3,0) */
		  jointDef3.localAnchorB.Set(3.0f,0.0f);
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef3.collideConnected = false;
		/*! enable limit is set to true in order to limit the rotation */
		  jointDef3.enableLimit = true;
		/*! The lower limit is set to -10 degrees, which is converted to radians using DegToRad variable defined earlier */
		  jointDef3.lowerAngle = -10*DegToRad ;
		/*! The upper limit is set to 30 degrees, which is converted to radians using DegToRad variable defined earlier */
		  jointDef3.upperAngle = 30*DegToRad ;
		/*! motor is enabled since it should be alowed to rotate */
		  jointDef3.enableMotor = true;
		/*! Maximum motor torque used to achieve the desired motor speed is set to 1000N-m */
		  jointDef3.maxMotorTorque = 1000;
		/*! Create the above mentioned joint in the world */ 
		  m_world->CreateJoint(&jointDef3);
		  
		  
/////////////////////claw
		/*! Define first part of the front claw with its position set at (-29,16.5) 
		* and body type set as dynamic body
		*/
		  b2BodyDef claw_bd;
		  claw_bd.position.Set(-29.0f, 16.5f);
		  claw_bd.type = b2_dynamicBody;
		/*! Create first part of the front claw in the world */
		  claw_b = m_world->CreateBody(&claw_bd);
		/*! Assign shape to first part of the front claw as a rectangle of length 4m and breadth 1m */
		  b2PolygonShape claw_s;
		  claw_s.SetAsBox(2.0f, 0.5f);  // original 2,5
		/*! Define fixtures with the above mentioned shape and with */
		/*! reative density = 0.1 */
		/*! coefficient of friction = 100
		*/
		  b2FixtureDef *claw_fd = new b2FixtureDef;
		  claw_fd->density = 0.1f;
		  claw_fd->shape = new b2PolygonShape;
		  claw_fd->shape = &claw_s;
		  claw_fd->friction = 100.0f;
		/*! Create the above mentioned fixtures on the body */
		  claw_b->CreateFixture(claw_fd);
		  
		/*! Create a joint, Revolute joint, between first part of front claw and second part of front arm */
		  b2RevoluteJointDef jointDef4;
		/*! The two bodies(BodyA, BodyB) between which joint is made are second part of the front arm
		* and first part of the front claw respectively 
		*/
		  jointDef4.bodyA = leftarmsub_b;
		  jointDef4.bodyB = claw_b;
		/*! the local anchor point relative to second part of the front arm's origin(-23,16.5) is set to (-4,0) */
		  jointDef4.localAnchorA.Set(-4.0,0);
		/*! the local anchor point relative to first part of the front claw's origin(-29,16.5) is set to (2,0) */
		  jointDef4.localAnchorB.Set(2.0f,0.0f);
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef4.collideConnected = false;
		/*! motor is enabled since it should be alowed to rotate */
		  jointDef4.enableMotor = true;
		/*! Maximum motor torque used to achieve the desired motor speed is set to 1000N-m */
		  jointDef4.maxMotorTorque = 1000;
         // jointDef4.motorSpeed = 0;//360 * DegToRad;
		/*! enable limit is set to true in order to limit the rotation */
		  jointDef4.enableLimit = true;
		/*! The lower limit is set to -75 degrees, which is converted to radians using DegToRad variable defined earlier */
		  jointDef4.lowerAngle = -75*DegToRad ;
		/*! The upper limit is set to 75 degrees, which is converted to radians using DegToRad variable defined earlier */
		  jointDef4.upperAngle = 75*DegToRad ;
		/*! Create the above mentioned joint in the world */ 
		  m_world->CreateJoint(&jointDef4);
		  
//////////////////////claw top
		/*! Define second part of the front claw with its position set at (-31,18) 
		* and body type set as dynamic body
		*/
		  b2BodyDef claw1_bd;
		  claw1_bd.position.Set(-31.0f, 18.0f); // -29.5,18
		  claw1_bd.type = b2_dynamicBody;
		/*! Create second part of the front claw in the world */
		  b2Body* claw1_b = m_world->CreateBody(&claw1_bd);
		/*! Assign shape to second part of the front claw as a rectangle of horizontal width 1m and vertical width4m */
		  b2PolygonShape claw1_s;
		  claw1_s.SetAsBox(0.5f, 2.0f);
		/*! Define fixtures with the above mentioned shape and with */
		/*! reative density = 0.1 */
		/*! coefficient of friction = 100 */
		  b2FixtureDef *claw1_fd = new b2FixtureDef;
		  claw1_fd->density = 0.1f;
		  claw1_fd->shape = new b2PolygonShape;
		  claw1_fd->shape = &claw1_s;
		/*! Create the above mentioned fixtures on the body */
		  claw1_b->CreateFixture(claw1_fd);
		  
		/*! Create a joint, weld joint(not free to rotate), between first part and second part of front claw */
		  b2WeldJointDef jointDef5;
		/*! The two bodies(BodyA, BodyB) between which joint is made are first part and second part
		* of the front claw respectively 
		*/
		  jointDef5.bodyA = claw_b;
		  jointDef5.bodyB = claw1_b;
		/*! the local anchor point relative to first part of the front claw's origin(-29,16.5) is set to (1,0) */
		  jointDef5.localAnchorA.Set(1.0f,0);
		/*! the local anchor point relative to second part of the front claw's origin(-31,18) is set to (0,-1.5) */
		  jointDef5.localAnchorB.Set(0.0f,-1.5f);
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef5.collideConnected = false;
		  //jointDef4.enableLimit = true;
		  //jointDef4.lowerAngle = -10*DegToRad ;
		  //jointDef4.upperAngle = 30*DegToRad ;
		/*! Create the above mentioned joint in the world */ 
		  m_world->CreateJoint(&jointDef5);    
		  
		/*  b2RevoluteJointDef jointDef7;
		  jointDef7.bodyA = leftarmsub_b;
		  jointDef7.bodyB = claw1_b;
		  jointDef7.localAnchorA.Set(-4.0f,0.0f);//0,1.5
		  jointDef7.localAnchorB.Set(0.0f,-1.5f);//-4,0
		  jointDef7.collideConnected = false;
		  //jointDef4.enableLimit = true;
		  //jointDef4.lowerAngle = -10*DegToRad ;
		  //jointDef4.upperAngle = 30*DegToRad ;
		  m_world->CreateJoint(&jointDef7); */
		  
////////////////////// upper dabba 
		/*! Define upper part of the main body of bulldozer with its position set to (5.5,21.5)
		* and body type set to dynnamic body.
		*/ 
		  b2BodyDef upperdabba_bd;
		  upperdabba_bd.type = b2_dynamicBody; 
		  float y_len = 10;
		  upperdabba_bd.position.Set(5.5f , 16.5 + y_len/2); 
		/*! Create the body in the world */
		  b2Body* upperdabba_body = m_world->CreateBody(&upperdabba_bd);
		/*! Make shape to the upper part of the bulldozer as a rectangle of length 19m and breadth 10m */ 
		  b2PolygonShape upperdabba_shape;
		  upperdabba_shape.SetAsBox(9.5f, (y_len)/2); // rectangle of 16x10 unit	
		/*! Define fixtures of the body with the above mentioned shape and with */
		/*! Relative density = 20 */
		/*! Coefficient of friction = 0.1
		*/
		  b2FixtureDef upperdabba_fd;
		  upperdabba_fd.shape = &upperdabba_shape;
		  upperdabba_fd.density = 20.0f;
		  upperdabba_fd.friction = 0.1f;
		/*! Create fixtures on the body	*/	
		  upperdabba_body->CreateFixture(&upperdabba_fd);
		  
		/*! Create a joint, weld joint(hence cannot rotate), between the predefined main body 
		* and upper part of the main body */
		  b2WeldJointDef jointDef6;
		/*! The two bodies(BodyA, BodyB) between which joint is made are upper part and lower part
		* of the main body respectively 
		*/
		  jointDef6.bodyA = upperdabba_body;
		  jointDef6.bodyB = main_body;
		/*! the local anchor point relative to upper part of the main body's origin(5.5,21.5) is set to (0,-5) */
		  jointDef6.localAnchorA.Set(0.0f,-y_len/2); // 0 , -y/2
		/*! the local anchor point relative to lower part of the main body's origin(0,10.5) is set to (5.5,6) */
		  jointDef6.localAnchorB.Set(5.5,6);// 5.5,6
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef6.collideConnected = false;
		/*! Create the above mentioned joint in the world */ 
		   m_world->CreateJoint(&jointDef6);
		  //body->SetLinearVelocity(b2Vec2( 3.0f, 0)); // giving linear velocity to the main body
		 
/////////////////////////////wall
		
		/*! Define the left wall with position at (-80,20) */   
		  b2BodyDef wall_bd;
		  //wall_bd.type = b2_dynamicBody; 
		  wall_bd.position.Set(-80.0f , 20.0f); 
		/*! Create the body in the world */
		  b2Body* wall_body = m_world->CreateBody(&wall_bd);
		/*! Make the shape of the left wall as a rectangle with */
		/*! horizontal width = 1m */
		/*! vertical width = 40m */
		  b2PolygonShape wall_shape;
		  wall_shape.SetAsBox(0.5f, 20.0f);
		/*! Define fixtures with the above mentioned shape and with */
		/*! relative density = 10000 */
		/*! coefficient of friction = 10000 */	
		  b2FixtureDef wall_fd;  
		  wall_fd.shape = &wall_shape;
		  wall_fd.density = 10000.0f;
		  wall_fd.friction = 10000.0f;
		/*! Create above mentioned fixtures on the body */ 
		  wall_body->CreateFixture(&wall_fd);
		  
		  
////////////////////////////boxes
		/*! Create the shape of small cubes that are to be carried by the bulldozer */
		/*! Each small box is a square of side length = 0.5m */
		  float side = 0.25f;		  
		  b2PolygonShape ball_s;
		  ball_s.SetAsBox(side, side); 
		/*! Define fixtures with the above mentioned shape and with */
		/*! relative density = 0.5 */
		/*! Coefficient of friction = 100 */
		/*! coefficient of resitution = 0, perfectly inelastic collision */
		  b2FixtureDef ball_fd;
		  ball_fd.shape = &ball_s;
		  ball_fd.density = 0.5f;// 1 for left side
		  ball_fd.friction = 100.0f;
		  ball_fd.restitution = 0.0f;
		  
		/*! 20 X 20 (=400) small cubes are placed beside the left wall which can be carried using bulldozer */
		  for (int i = 0; i < 20; i++)
		  {
			/*! for integer 'i' in range 0 - 19 */
			  for (int j = 0; j < 20; j++)
			  {
				/*! for integer 'j' in range 0 - 19 */
				/*! Define box(i,j) with position set at (-79+(i)*0.5,0.25+(j)*0.5) 
				* and the body type is set as Dynamic body */
				  b2BodyDef ball_bd;
				  ball_bd.type = b2_dynamicBody;
				  ball_bd.position.Set(-79.0f + (i)*2*side, side + (j)*2*side);
				/*! Create box(i,j) in the world */
				  b2Body* ball_b = m_world->CreateBody(&ball_bd);
				/*! As all boxes are identical, fixtures of all boxes are same */
				/*! Create the predefined fixtures of small cube on the body */
				  ball_b->CreateFixture(&ball_fd);
			 }
		  }
		  
		/*   for (int i = 0; i < 20; i++)
		  {
			  for (int j = 0; j < 20; j++)
			  {
				  
				  b2BodyDef ball_bd;
				  ball_bd.type = b2_dynamicBody;
				  ball_bd.position.Set(50.0f + (i)*2*side, side + (j)*2*side);
				  b2Body* ball_b = m_world->CreateBody(&ball_bd);
				  ball_b->CreateFixture(&ball_fd);
			 }
		  } */
		  
/////////////////////front claw-type
		/*! Define third part of the front claw with its position set at (-31.5,16.5) 
		* and body type set as dynamic body
		*/
		  b2BodyDef poke_bd;
		  poke_bd.type = b2_dynamicBody;
		  poke_bd.position.Set(-31.5f , 16.5f); 
		/*! Create third part of the front claw in the world */
		  b2Body* poke_body = m_world->CreateBody(&poke_bd);
		/*! Assign shape to third part of the front claw as a polygon with vertices
		* at (0.5,0.5), (-0.5,1.5), (-0.5,-0.5) and (+0.5,-0.5) with respect to origin. */
		  b2PolygonShape poke_s;
	      	  b2Vec2 vertice[4];
		  vertice[0].Set(0.5,0.5);
		  vertice[1].Set(-0.5,1.5);
		  vertice[2].Set(-0.5,-0.5);
		  vertice[3].Set(+0.5,-0.5);
		  poke_s.Set(vertice, 4);
		
		/*! Define fixtures with the above mentioned shape and with */
		/*! reative density = 0.01 */
		/*! coefficient of friction = 0.1
		*/  
		  b2FixtureDef poke_fd;
		  poke_fd.shape = &poke_s;
		  poke_fd.density = 0.01f;
		  poke_fd.friction = 0.1f;
			
		/*! Create the above mentioned fixtures on the body */  
		  poke_body->CreateFixture(&poke_fd);
		
		/*! Create a joint, weld joint(not free to rotate), between third part and second part of front claw */  
		   b2WeldJointDef jointDef8;
		/*! The two bodies(BodyA, BodyB) between which joint is made are third part and second part
		* of the front claw respectively 
		*/
		  jointDef8.bodyA = poke_body;
		  jointDef8.bodyB = claw_b;
		/*! the local anchor point relative to third part of the front claw's origin(-31.5,16.5) is set to (0.5,0) */
		  jointDef8.localAnchorA.Set(0.5f,0); 
		/*! the local anchor point relative to second part of the front claw's origin(-31,18) is set to (-2,0) */
		  jointDef8.localAnchorB.Set(-2,0);
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef8.collideConnected = false;
		/*! Create the above mentioned joint in the world */ 
		   m_world->CreateJoint(&jointDef8);
		   
/////////////////////right arm
		/*! Define the first part of rear arm of the bulldozer */
		  b2BodyDef rightarm_bd;
		/*! set its body type to dynamicbody */
		  rightarm_bd.type = b2_dynamicBody;
		/*! set its position at (14.5,5) */
		  rightarm_bd.position.Set(14.5f , 5.0f); 
		/*! create the first part of rear arm in the world */
		  rightarm_body = m_world->CreateBody(&rightarm_bd);
		/*! Assign the shape of first part of the rear arm as a polygon with vertices
		* (-1.5,1), (-1.5,-1), (0.5,-1), (7.5,26) and (7.5,29) in the given order */
		  b2PolygonShape rightarm_s;
	      	  b2Vec2 vertex[5];
		  vertex[0].Set(-1.5,1);
		  vertex[1].Set(-1.5,-1);
		  vertex[2].Set(0.5,-1);
		  vertex[3].Set(7.5,26);
		  vertex[4].Set(7.5,29);
		  rightarm_s.Set(vertex, 5);	
			
		/*! Define the fixtures of first part of the rear arm with the above mentioned shape and with */
		/*! Relative density = 0.1 */
		/*! coefficient of friction = 0.1 */ 	
		  b2FixtureDef rightarm_fd;
		  rightarm_fd.shape = &rightarm_s;
		  rightarm_fd.density = 0.1f;
		  rightarm_fd.friction = 0.1f;
			
		/*! Create above mentioned fixtures to first part of the rear arm body */  
		  rightarm_body->CreateFixture(&rightarm_fd);
		
		/*! Create a joint, Revolute joint, between the main body and first part of the rear arm */
		  b2WeldJointDef jointDef9;
		/*! The two bodies(BodyA, BodyB) between which joint is made are main body and first part of the rear arm respectively */
		  jointDef9.bodyA = main_body;
		  jointDef9.bodyB = rightarm_body;
		/*! the local anchor point relative to main body's origin(0,10.5) is set to (14.5,-5.5) */
		  jointDef9.localAnchorA.Set(14.5,-5.5);
		/*! the local anchor point relative to first part of rear arm's origin(14.5,5) is set to (0,0) */
		  jointDef9.localAnchorB.Set(0.0f,0.0f);
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef9.collideConnected = false;
		 /* jointDef9.enableLimit = true;
		  jointDef9.lowerAngle = -20*DegToRad ;
		  //jointDef9.upperAngle = 30*DegToRad ;
		  jointDef9.enableMotor = true;
		  jointDef9.maxMotorTorque = 5000; */
		/*! Create the above mentioned joint in the world */ 
		  m_world->CreateJoint(&jointDef9); 
		  
		  
////////////////////right arm sub
		
		/*! Define the second part of rear arm of the bulldozer */  
		  b2BodyDef rightarmsub_bd;
		/*! set its body type to dynamicbody */
		  rightarmsub_bd.type = b2_dynamicBody;
		/*! set its position at (35,32.5) */ 
		  rightarmsub_bd.position.Set(35.0f , 32.5f);
		/*! create the second part of rear arm in the world */ 
		  rightarmsub_body = m_world->CreateBody(&rightarmsub_bd);
		/*! Assign the shape of second part of the rear arm as a polygon with vertices at
		* (-13.0,1.5), (-13.0,-1.5), (+13.0,0.5) and (+13.0,-0.5) in the given order. */
		  b2PolygonShape rightarmsub_s;
	      //rightarmsub_s.SetAsBox(10.0f, 1.0f);
		  
		  
		  b2Vec2 vertic[4];
		  vertic[0].Set(-13.0,1.5);
		  vertic[1].Set(-13.0,-1.5);
		  vertic[2].Set(+13.0,0.5);
		  vertic[3].Set(+13.0,-0.5);
		  rightarmsub_s.Set(vertic, 4);
		/*! Define the fixtures of second part of the rear arm with the above mentioned shape and with */
		/*! Relative density = 0.1 */ 
		/*! coefficient of friction = 0.1 */  	
		  b2FixtureDef rightarmsub_fd;
		  rightarmsub_fd.shape = &rightarmsub_s;
		  rightarmsub_fd.density = 0.1f;
		  rightarmsub_fd.friction = 0.1f;
			
		/*! Create above mentioned fixtures to second part of the rear arm body */  
		  rightarmsub_body->CreateFixture(&rightarmsub_fd);
		/*! Create a joint, Revolute joint, between first part and second part of the rear arm */  
		  b2RevoluteJointDef jointDef10;
		/*! The two bodies(BodyA, BodyB) between which joint is made are first part and second part 
		* of the rear arm respectively 
		*/
		  jointDef10.bodyA = rightarm_body;
		  jointDef10.bodyB = rightarmsub_body;
		/*! the local anchor point relative to first part of the rear arm's origin(14.5,5) is set to (7.5,27.5) */
		  jointDef10.localAnchorA.Set(7.5,27.5);
		/*! the local anchor point relative to second part of the rear arm's origin(35,32.5) is set to (-13,0) */
		  jointDef10.localAnchorB.Set(-13.0f,0.0f);
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef10.collideConnected = false;
		/*! enable limit is set to true in order to limit the rotation */
		  jointDef10.enableLimit = true;
		/*! The lower limit is set to -60 degrees, which is converted to radians using DegToRad variable defined earlier */
		  jointDef10.lowerAngle = -60*DegToRad ;
		  //jointDef10.upperAngle = 30*DegToRad ;
		/*! motor is enabled since it should be alowed to rotate */
		  jointDef10.enableMotor = true;
		/*! Maximum motor torque used to achieve the desired motor speed is set to 5000N-m */
		  jointDef10.maxMotorTorque = 5000;
		/*! Create the above mentioned joint in the world */ 
		  m_world->CreateJoint(&jointDef10); 
		  
		  
///////////////////// right claw
		/*! Define first part of the rear claw with its position set at (48.5,29) 
		* and body type set as dynamic body
		*/
		  b2BodyDef rightclaw_bd;
		  rightclaw_bd.type = b2_dynamicBody;
		  rightclaw_bd.position.Set(48.5f , 29.0f); 
		/*! Create first part of the rear claw in the world */
		  rightclaw_body = m_world->CreateBody(&rightclaw_bd); 
		/*! Assign shape to first part of the rear claw as a rectangle of */
		/*! horizontal width = 1m */
		/*! vertical width = 8m */
		  b2PolygonShape rightclaw_s;
		  rightclaw_s.SetAsBox(0.5f, 4.0f);	
		  
		/*! Define fixtures with the above mentioned shape and with */
		/*! reative density = 0.1 */
		/*! coefficient of friction = 100 */
		/*! coeffcient of restitution = 0, perfectly inelastic body.*/
		  b2FixtureDef rightclaw_fd;
		  rightclaw_fd.shape = &rightclaw_s;
		  rightclaw_fd.density = 0.1f;
		  rightclaw_fd.friction = 100.0f;
		  ball_fd.restitution = 0.0f;
			
		/*! Create the above mentioned fixtures on the body */ 
		  rightclaw_body->CreateFixture(&rightclaw_fd);
		/*! Create a joint, Revolute joint, between first part of rear claw and second part of rear arm */  
		  b2RevoluteJointDef jointDef11;
		/*! The two bodies(BodyA, BodyB) between which joint is made are second part of the rear arm
		* and first part of the rear claw respectively 
		*/
		  jointDef11.bodyA = rightarmsub_body;
		  jointDef11.bodyB = rightclaw_body;
		/*! the local anchor point relative to second part of the rear arm's origin(35,32.5) is set to (13.5,0) */
		  jointDef11.localAnchorA.Set(13.5,0);
		/*! the local anchor point relative to first part of the rear claw's origin(48.5,29) is set to (0,3.5) */
		  jointDef11.localAnchorB.Set(0.0f,3.5f);
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef11.collideConnected = false;
		/*! enable limit is set to true in order to limit the rotation */
		  jointDef11.enableLimit = true;
		/*! The lower limit is set to -30 degrees, which is converted to radians using DegToRad variable defined earlier */
		  jointDef11.lowerAngle = -30*DegToRad ;
		/*! The upper limit is set to 65 degrees, which is converted to radians using DegToRad variable defined earlier */
		  jointDef11.upperAngle = 65*DegToRad ;
		/*! motor is enabled since it should be alowed to rotate */
		  jointDef11.enableMotor = true;
		/*! Maximum motor torque used to achieve the desired motor speed is set to 1000N-m */
		  jointDef11.maxMotorTorque = 1000;
		/*! Create the above mentioned joint in the world */ 
		  m_world->CreateJoint(&jointDef11);
		  
//////////////////////right spoke
		/*! Define second part of the rear claw with its position set at (48.5,24.5) 
		* and body type set as dynamic body
		*/
		  b2BodyDef rightspoke_bd;
		  rightspoke_bd.type = b2_dynamicBody;
		  rightspoke_bd.position.Set(48.5f , 24.5f);
		/*! Create second part of the rear claw in the world */ 
		  b2Body* rightspoke_body = m_world->CreateBody(&rightspoke_bd);
		/*! Assign shape to second part of the rear claw as a polygon with vertices at 
		* (-5,0.5), (-5.0,-0.5), (+0.5,-0.5) and (+0.5,0.5) in the given order. */
		  b2PolygonShape rightspoke_s;	
		  b2Vec2 verti[4];
		  verti[0].Set(-5,0.5);
		  verti[1].Set(-5.0,-0.5);
		  verti[2].Set(+0.5,-0.5);
		  verti[3].Set(+0.5,0.5);
		  rightspoke_s.Set(verti, 4);
		  
		/*! Define fixtures with the above mentioned shape and with */
		/*! reative density = 0.1 */
		/*! coefficient of friction = 100 */ 
		/*! coeffcient of restitution = 0, perfectly inelastic body.*/
		  b2FixtureDef rightspoke_fd;
		  rightspoke_fd.shape = &rightspoke_s;
		  rightspoke_fd.density = 0.1f;
		  rightspoke_fd.friction = 100.0f;
		  rightspoke_fd.restitution = 0.0f;
			
		/*! Create the above mentioned fixtures on the body */
		  rightspoke_body->CreateFixture(&rightspoke_fd);
		/*! Create a joint, weld joint(not free to rotate), between first part and second part of rear claw */  
		  b2WeldJointDef jointDef12;
		/*! The two bodies(BodyA, BodyB) between which joint is made are first part and second part
		* of the rear claw respectively 
		*/
		  jointDef12.bodyA = rightclaw_body;
		  jointDef12.bodyB = rightspoke_body;
		/*! the local anchor point relative to first part of the front claw's origin(48.5,29) is set to (0,-4) */
		  jointDef12.localAnchorA.Set(0.0f,-4.0f);
		/*! the local anchor point relative to second part of the front claw's origin(48.5,24.5) is set to (0,0.5) */
		  jointDef12.localAnchorB.Set(0.0f,0.5f);
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef12.collideConnected = false;
		/*! Create the above mentioned joint in the world */ 
		   m_world->CreateJoint(&jointDef12);
		  
		/*  // right object to pick
		  
		  b2PolygonShape vert_shape;
		  vert_shape.SetAsBox(0.5f, 1.5f); 
			
			
		  b2FixtureDef vert_fd;
		  vert_fd.shape = &vert_shape;
		  vert_fd.density = 0.1f;
		  vert_fd.friction = 100.0f;
			
		  b2BodyDef vert_bd;
		  vert_bd.type = b2_dynamicBody; 
		  vert_bd.position.Set(40 , 1.5f); 
		  b2Body* vert_body = m_world->CreateBody(&vert_bd);
		  vert_body->CreateFixture(&vert_fd);
		  
		  b2PolygonShape vert_shape1;
		  vert_shape1.SetAsBox(4, 0.5f); 
			
			
		  b2FixtureDef vert_fd1;
		  vert_fd1.shape = &vert_shape1;
		  vert_fd1.density = 0.1f;
		  vert_fd1.friction = 100.0f;
		  
		  b2Body* vert_body1 = m_world->CreateBody(&vert_bd);
		  vert_body1->CreateFixture(&vert_fd1);
		    
		  b2WeldJointDef jointDef13;
		  jointDef13.bodyA = vert_body;
		  jointDef13.bodyB = vert_body1;
		  jointDef13.localAnchorA.Set(0.0f,1.5f); 
		  jointDef13.localAnchorB.Set(0,0);
		  jointDef13.collideConnected = false;
		   m_world->CreateJoint(&jointDef13);
		   
		   b2Body* vert_body2 = m_world->CreateBody(&vert_bd);
		  vert_body2->CreateFixture(&vert_fd1);
		    
		  b2WeldJointDef jointDef14;
		  jointDef14.bodyA = vert_body;
		  jointDef14.bodyB = vert_body2;
		  jointDef14.localAnchorA.Set(0.0f,-1.5f); 
		  jointDef14.localAnchorB.Set(0,0);
		  jointDef14.collideConnected = false;
		   m_world->CreateJoint(&jointDef14); 
		  */
		  
//////////////////sright box's and wall
		  
		/*! Create the shape of small cubes that are to be carried by the bulldozer */
		/*! Each small box is a square of side length = 0.5m */
		  b2PolygonShape box_shape;
		  box_shape.SetAsBox(0.25f, 0.25f); 
			
		/*! Define fixtures with the above mentioned shape and with */
		/*! relative density = 0.5 */
		/*! Coefficient of friction = 100 */
		/*! coefficient of resitution = 0, perfectly inelastic collision */	
		  b2FixtureDef box_fd;
		  box_fd.shape = &box_shape;
		  box_fd.density = 0.5f;
		  box_fd.friction = 100.0f;
		  box_fd.restitution = 0.0f;
		
		/*! 20 X 20 (=400) small cubes are placed beside the right wall which can be carried using bulldozer */	
		  for (int i = 0; i < 15; i++)
		  {
			/*! for integer 'i' in range 0 - 19 */
			  for (int j = 0; j < 15; j++)
			{
				/*! for integer 'j' in range 0 - 19 */
				/*! Define box(i,j) with position set at (60+(i)*0.5,1+(j)*0.5) 
				* and the body type is set as Dynamic body */  
			  b2BodyDef box_bd;
			 box_bd.type = b2_dynamicBody;
			box_bd.position.Set(60.0f + (i)*2*0.25, 1 + (j)*2*0.25);
				/*! Create box(i,j) in the world */
				  b2Body* box_b = m_world->CreateBody(&box_bd);
				/*! As all boxes are identical, fixtures of all boxes are same */
				/*! Create the predefined fixtures of small cube on the body */
				  box_b->CreateFixture(&box_fd);
			 }
		  }
		  
		  
		/*! Define the right wall with position at (58.5,5) */   
		  b2BodyDef wall1_bd;
		  //wall1_bd.type = b2_dynamicBody;
		  wall1_bd.position.Set(58.5, 5);
		/*! Create the body in the world */
		  b2Body* wall1_b = m_world->CreateBody(&wall1_bd); 
		/*! Make the shape of the left wall as a rectangle with */
		/*! horizontal width = 2m */
		/*! vertical width = 10m */ 
		  b2PolygonShape wall1_shape;
		  wall1_shape.SetAsBox(1.0f, 5.0f); 
			
		/*! Define fixtures with the above mentioned shape and with */
		/*! relative density = 1000 */
		/*! coefficient of friction = 1000 */		
		  b2FixtureDef wall1_fd;
		  wall1_fd.shape = &wall1_shape;
		  wall1_fd.density = 1000.0f;
		  wall1_fd.friction = 1000.0f;
		  
		/*! Create above mentioned fixtures on the body */   
		  wall1_b->CreateFixture(&wall1_fd);
		  
		  
/////////////////// right extra spoke
		  
		/*! Define third part of the rear claw with its position set at (43,24.5) 
		* and body type set as dynamic body
		*/  
		  b2BodyDef poke1_bd;
		  poke1_bd.type = b2_dynamicBody;
		  poke1_bd.position.Set(43.0f , 24.5f); 
		/*! Create third part of the rear claw in the world */
		  b2Body* poke1_body = m_world->CreateBody(&poke1_bd);
		/*! Assign shape to third part of the rear claw as a polygon with vertices
		* at (0.5,0.5), (-0.5,1.5), (-0.5,-0.5) and (+0.5,-0.5) with respect to origin. */
		  b2PolygonShape poke1_s;
	      	  b2Vec2 vertice1[4];
		  vertice1[0].Set(0.5,0.5);
		  vertice1[1].Set(-0.5,1.5);
		  vertice1[2].Set(-0.5,-0.5);
		  vertice1[3].Set(+0.5,-0.5);
		  poke1_s.Set(vertice1, 4);
		
		/*! Define fixtures with the above mentioned shape and with */
		/*! reative density = 0.01 */
		/*! coefficient of friction = 0.1
		*/  
		  b2FixtureDef poke1_fd;
		  poke1_fd.shape = &poke1_s;
		  poke1_fd.density = 0.01f;
		  poke1_fd.friction = 0.1f;
			
		/*! Create the above mentioned fixtures on the body */    
		  poke1_body->CreateFixture(&poke1_fd);
		
		/*! Create a joint, weld joint(not free to rotate), between third part and second part of front claw */    
		  b2WeldJointDef jointDef15;
		/*! The two bodies(BodyA, BodyB) between which joint is made are third part and second part
		* of the rear claw respectively 
		*/
		  jointDef15.bodyA = poke1_body;
		  jointDef15.bodyB = rightspoke_body;
		/*! the local anchor point relative to third part of the rear claw's origin(43,24.5) is set to (0.5,0) */
		  jointDef15.localAnchorA.Set(0.5f,0);
		/*! the local anchor point relative to second part of the rear claw's origin(48.5,24.5) is set to (-5,0) */
		  jointDef15.localAnchorB.Set(-5,0);
		/*! collide connected is set to false since the two bodies do not collide */ 
		  jointDef15.collideConnected = false;
		/*! Create the above mentioned joint in the world */ 
		   m_world->CreateJoint(&jointDef15);
		   
		}
   } 
   
  }

  sim_t *sim = new sim_t("Project", dominos_t::create);
}
