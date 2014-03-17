
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
	  float DegToRad = 0.0174532925;
	  // main ground 
    {
      
      b2EdgeShape shape; 
      shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f));
      b2BodyDef bd; 
      b2Body* b1 = m_world->CreateBody(&bd); 
      b1->CreateFixture(&shape, 0.0f);
    }
	{  // main block
		 {
		  //b2PolygonShape shape;
		 // shape.SetAsBox(15.0f, 7.5f); // rectangle of 16x10 units
		  b2PolygonShape main_s;
	      b2Vec2 vertices[5];
		  vertices[0].Set(15,6);
		  vertices[4].Set(15,-7.5);
		  vertices[3].Set(-15,-7.5);
		  vertices[2].Set(-15,0);
		  vertices[1].Set(-4,6);
		  main_s.Set(vertices, 5);	
			
			
		  b2FixtureDef main_fd;
		  main_fd.shape = &main_s;
		  main_fd.density = 20.0f;
		  main_fd.friction = 0.1f;
			
		  b2BodyDef main_bd;
		  main_bd.type = b2_dynamicBody;
		  main_bd.position.Set(0.0f , 10.5f); 
		  main_body = m_world->CreateBody(&main_bd);
		  main_body->CreateFixture(&main_fd);
		  
		//  body->SetLinearVelocity(b2Vec2( 3.0f, 0)); // giving linear velocity to the main body
		 
		  //The Bigger tyre
		 
		  b2CircleShape circle;
		  circle.m_radius = 5.0;
		
		  b2FixtureDef ballfd;
		  ballfd.shape = &circle;
		  ballfd.density = 5.0f;
		  ballfd.friction = 0.1f;
		  ballfd.restitution = 0.0f;
		  b2BodyDef ballbd;
		  ballbd.type = b2_dynamicBody;
		  ballbd.position.Set(7.0f, 5.0f);
		  b2Body* sbody = m_world->CreateBody(&ballbd);
		  sbody->CreateFixture(&ballfd);
		  
		  b2RevoluteJointDef jointDef;
		  jointDef.bodyA = main_body;
		  jointDef.bodyB = sbody;
		  jointDef.localAnchorA.Set(7,-5.5);
		  jointDef.localAnchorB.Set(0,0);
		  jointDef.collideConnected = false;
		  jointDef.enableMotor = true;
		  jointDef.maxMotorTorque = 500;
		  m_world->CreateJoint(&jointDef);
		  
		  
		  // small tyre
		  b2CircleShape circle1;
		  circle1.m_radius = 3.0;
		
		  b2FixtureDef ballfd1;
		  ballfd1.shape = &circle1;
		  ballfd1.density = 5.0f;
		  ballfd1.friction = 0.1f;
		  ballfd1.restitution = 0.0f;
		  b2BodyDef ballbd1;
		  ballbd1.type = b2_dynamicBody;
		  ballbd1.position.Set(-7.0f, 3.0f);
		  b2Body* sbody1 = m_world->CreateBody(&ballbd1);
		  sbody1->CreateFixture(&ballfd1);
		  
		  b2RevoluteJointDef jointDef1;
		  jointDef1.bodyA = main_body;
		  jointDef1.bodyB = sbody1;
		  jointDef1.localAnchorA.Set(-7,-7.5);
		  jointDef1.localAnchorB.Set(0,0);
		  jointDef1.collideConnected = false;
		  jointDef1.enableMotor = true;
		  jointDef1.maxMotorTorque = 500;
		  m_world->CreateJoint(&jointDef1);
		  
		 //left arm
		  b2PolygonShape leftarm_s;
		  leftarm_s.SetAsBox(7.5f, 0.5f);
		  b2BodyDef leftarm_bd;
		  leftarm_bd.position.Set(-11.5f, 16.5f);
		  leftarm_bd.type = b2_dynamicBody;
		  leftarm_b = m_world->CreateBody(&leftarm_bd);
		  b2FixtureDef *leftarm_fd = new b2FixtureDef;
		  leftarm_fd->density = 1.f;
		  leftarm_fd->shape = new b2PolygonShape;
		  leftarm_fd->shape = &leftarm_s;
		  leftarm_b->CreateFixture(leftarm_fd);
		  
		  b2RevoluteJointDef jointDef2;
		  jointDef2.bodyA = leftarm_b;
		  jointDef2.bodyB = main_body;
		  jointDef2.localAnchorA.Set(7.5,0);
		  jointDef2.localAnchorB.Set(-4.0f,6.0f);
		  jointDef2.collideConnected = false;
		  jointDef2.enableLimit = true;
		  jointDef2.lowerAngle = -60*DegToRad ;
		//  jointDef2.upperAngle = 0*DegToRad ;
		  jointDef2.enableMotor = true;
		  jointDef2.maxMotorTorque = 8000;
        //  jointDef2.motorSpeed = 0;//360 * DegToRad;
		  m_world->CreateJoint(&jointDef2);
		  
		  
		  //leftarm_b->ApplyLinearImpulse( b2Vec2(-5,1000), leftarm_b->GetWorldCenter() , true );
		  
		  //left arm sub
		  b2PolygonShape leftarmsub_s;
		  leftarmsub_s.SetAsBox(4.0f, 0.5f);
		  b2BodyDef leftarmsub_bd;
		  leftarmsub_bd.position.Set(-23.0f, 16.5f);
		  leftarmsub_bd.type = b2_dynamicBody;
		  b2Body* leftarmsub_b = m_world->CreateBody(&leftarmsub_bd);
		  b2FixtureDef *leftarmsub_fd = new b2FixtureDef;
		  leftarmsub_fd->density = 1.f;
		  leftarmsub_fd->shape = new b2PolygonShape;
		  leftarmsub_fd->shape = &leftarmsub_s;
		  leftarmsub_b->CreateFixture(leftarmsub_fd);
		  
		   b2RevoluteJointDef jointDef3;
		  jointDef3.bodyA = leftarm_b;
		  jointDef3.bodyB = leftarmsub_b;
		  jointDef3.localAnchorA.Set(-7.5,0);
		  jointDef3.localAnchorB.Set(3.0f,0.0f);
		  jointDef3.collideConnected = false;
		  jointDef3.enableLimit = true;
		  jointDef3.lowerAngle = -10*DegToRad ;
		  jointDef3.upperAngle = 30*DegToRad ;
		  jointDef3.enableMotor = true;
		  jointDef3.maxMotorTorque = 1000;
		  m_world->CreateJoint(&jointDef3);
		  
		  
		  //claw
		  b2PolygonShape claw_s;
		  claw_s.SetAsBox(2.0f, 0.5f);  // original 2,5
		  b2BodyDef claw_bd;
		  claw_bd.position.Set(-29.0f, 16.5f);
		  claw_bd.type = b2_dynamicBody;
		  claw_b = m_world->CreateBody(&claw_bd);
		  b2FixtureDef *claw_fd = new b2FixtureDef;
		  claw_fd->density = 0.1f;
		  claw_fd->shape = new b2PolygonShape;
		  claw_fd->shape = &claw_s;
		  claw_fd->friction = 100.0f;
		  claw_b->CreateFixture(claw_fd);
		  
		   b2RevoluteJointDef jointDef4;
		  jointDef4.bodyA = leftarmsub_b;
		  jointDef4.bodyB = claw_b;
		  jointDef4.localAnchorA.Set(-4.0,0);
		  jointDef4.localAnchorB.Set(2.0f,0.0f);
		  jointDef4.collideConnected = false;
		  jointDef4.enableMotor = true;
		  jointDef4.maxMotorTorque = 1000;
         // jointDef4.motorSpeed = 0;//360 * DegToRad;
		  jointDef4.enableLimit = true;
		  jointDef4.lowerAngle = -75*DegToRad ;
		  jointDef4.upperAngle = 75*DegToRad ;
		  m_world->CreateJoint(&jointDef4);
		  
	  //claw top
		  b2PolygonShape claw1_s;
		  claw1_s.SetAsBox(0.5f, 2.0f);
		  b2BodyDef claw1_bd;
		  claw1_bd.position.Set(-31.0f, 18.0f); // -29.5,18
		  claw1_bd.type = b2_dynamicBody;
		  b2Body* claw1_b = m_world->CreateBody(&claw1_bd);
		  b2FixtureDef *claw1_fd = new b2FixtureDef;
		  claw1_fd->density = 0.1f;
		  claw1_fd->shape = new b2PolygonShape;
		  claw1_fd->shape = &claw1_s;
		  claw1_b->CreateFixture(claw1_fd);
		  
		   b2WeldJointDef jointDef5;
		  jointDef5.bodyA = claw_b;
		  jointDef5.bodyB = claw1_b;
		  jointDef5.localAnchorA.Set(1.0f,0);
		  jointDef5.localAnchorB.Set(0.0f,-1.5f);
		  jointDef5.collideConnected = false;
		  //jointDef4.enableLimit = true;
		  //jointDef4.lowerAngle = -10*DegToRad ;
		  //jointDef4.upperAngle = 30*DegToRad ;
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
		  
		 // upper dabba 
		   b2PolygonShape upperdabba_shape;
		   float y_len = 10;
		   upperdabba_shape.SetAsBox(9.5f, (y_len)/2); // rectangle of 16x10 unit	
			
			
		  b2FixtureDef upperdabba_fd;
		  upperdabba_fd.shape = &upperdabba_shape;
		  upperdabba_fd.density = 20.0f;
		  upperdabba_fd.friction = 0.1f;
			
		  b2BodyDef upperdabba_bd;
		  upperdabba_bd.type = b2_dynamicBody; 
		  upperdabba_bd.position.Set(5.5f , 16.5 + y_len/2); 
		  b2Body* upperdabba_body = m_world->CreateBody(&upperdabba_bd);
		  upperdabba_body->CreateFixture(&upperdabba_fd);
		  
		  b2WeldJointDef jointDef6;
		  jointDef6.bodyA = upperdabba_body;
		  jointDef6.bodyB = main_body;
		  jointDef6.localAnchorA.Set(0.0f,-y_len/2); // 0 , -y/2
		  jointDef6.localAnchorB.Set(5.5,6);// 5.5,6
		  jointDef6.collideConnected = false;
		   m_world->CreateJoint(&jointDef6);
		  //body->SetLinearVelocity(b2Vec2( 3.0f, 0)); // giving linear velocity to the main body
		 
		  //wall
		   b2PolygonShape wall_shape;
		   wall_shape.SetAsBox(0.5f, 20.0f); 	
			
			
		  b2FixtureDef wall_fd;  
		  wall_fd.shape = &wall_shape;
		  wall_fd.density = 10000.0f;
		  wall_fd.friction = 10000.0f;
			
		  b2BodyDef wall_bd;
		  wall_bd.type = b2_dynamicBody; 
		  wall_bd.position.Set(-80.0f , 20.0f); 
		  b2Body* wall_body = m_world->CreateBody(&wall_bd);
		  wall_body->CreateFixture(&wall_fd);
		  
		  
		  //boxes
		  float side = 0.25f;		  
		  b2PolygonShape ball_s;
		  ball_s.SetAsBox(side, side); 
		
		  b2FixtureDef ball_fd;
		  ball_fd.shape = &ball_s;
		  ball_fd.density = 0.5f;// 1 for left side
		  ball_fd.friction = 100.0f;
		  ball_fd.restitution = 0.0f;
		  
		  
		     
		  
		  for (int i = 0; i < 20; i++)
		  {
			  for (int j = 0; j < 20; j++)
			  {
				  
				  b2BodyDef ball_bd;
				  ball_bd.type = b2_dynamicBody;
				  ball_bd.position.Set(-79.0f + (i)*2*side, side + (j)*2*side);
				  b2Body* ball_b = m_world->CreateBody(&ball_bd);
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
		  
		  //front claw-type
		  b2PolygonShape poke_s;
	      b2Vec2 vertice[4];
		  vertice[0].Set(0.5,0.5);
		  vertice[1].Set(-0.5,1.5);
		  vertice[2].Set(-0.5,-0.5);
		  vertice[3].Set(+0.5,-0.5);
		  poke_s.Set(vertice, 4);
		  
		  b2FixtureDef poke_fd;
		  poke_fd.shape = &poke_s;
		  poke_fd.density = 0.01f;
		  poke_fd.friction = 0.1f;
			
		  b2BodyDef poke_bd;
		  poke_bd.type = b2_dynamicBody;
		  poke_bd.position.Set(-31.5f , 16.5f); 
		  b2Body* poke_body = m_world->CreateBody(&poke_bd);
		  poke_body->CreateFixture(&poke_fd);
		  
		   b2WeldJointDef jointDef8;
		  jointDef8.bodyA = poke_body;
		  jointDef8.bodyB = claw_b;
		  jointDef8.localAnchorA.Set(0.5f,0); 
		  jointDef8.localAnchorB.Set(-2,0);
		  jointDef8.collideConnected = false;
		   m_world->CreateJoint(&jointDef8);
		   
		   //right arm
		   
		  b2PolygonShape rightarm_s;
	      b2Vec2 vertex[5];
		  vertex[0].Set(-1.5,1);
		  vertex[1].Set(-1.5,-1);
		  vertex[2].Set(0.5,-1);
		  vertex[3].Set(7.5,26);
		  vertex[4].Set(7.5,29);
		  rightarm_s.Set(vertex, 5);	
			
			
		  b2FixtureDef rightarm_fd;
		  rightarm_fd.shape = &rightarm_s;
		  rightarm_fd.density = 0.1f;
		  rightarm_fd.friction = 0.1f;
			
		  b2BodyDef rightarm_bd;
		  rightarm_bd.type = b2_dynamicBody;
		  rightarm_bd.position.Set(14.5f , 5.0f); 
		  rightarm_body = m_world->CreateBody(&rightarm_bd);
		  rightarm_body->CreateFixture(&rightarm_fd);
		  
		  b2WeldJointDef jointDef9;
		  jointDef9.bodyA = main_body;
		  jointDef9.bodyB = rightarm_body;
		  jointDef9.localAnchorA.Set(14.5,-5.5);
		  jointDef9.localAnchorB.Set(0.0f,0.0f);
		  jointDef9.collideConnected = false;
		 /* jointDef9.enableLimit = true;
		  jointDef9.lowerAngle = -20*DegToRad ;
		  //jointDef9.upperAngle = 30*DegToRad ;
		  jointDef9.enableMotor = true;
		  jointDef9.maxMotorTorque = 5000; */
		  m_world->CreateJoint(&jointDef9); 
		  
		  
		  //right arm sub
		  
		  
		  b2PolygonShape rightarmsub_s;
	      //rightarmsub_s.SetAsBox(10.0f, 1.0f);
		  
		  
	      b2Vec2 vertic[4];
		  vertic[0].Set(-13.0,1.5);
		  vertic[1].Set(-13.0,-1.5);
		  vertic[2].Set(+13.0,0.5);
		  vertic[3].Set(+13.0,-0.5);
		  rightarmsub_s.Set(vertic, 4);
		  	
		  b2FixtureDef rightarmsub_fd;
		  rightarmsub_fd.shape = &rightarmsub_s;
		  rightarmsub_fd.density = 0.1f;
		  rightarmsub_fd.friction = 0.1f;
			
		  b2BodyDef rightarmsub_bd;
		  rightarmsub_bd.type = b2_dynamicBody;
		  rightarmsub_bd.position.Set(35.0f , 32.5f); 
		  rightarmsub_body = m_world->CreateBody(&rightarmsub_bd);
		  rightarmsub_body->CreateFixture(&rightarmsub_fd);
		  
		  b2RevoluteJointDef jointDef10;
		  jointDef10.bodyA = rightarm_body;
		  jointDef10.bodyB = rightarmsub_body;
		  jointDef10.localAnchorA.Set(7.5,27);
		  jointDef10.localAnchorB.Set(-13.0f,0.0f);
		  jointDef10.collideConnected = false;
		  jointDef10.enableLimit = true;
		  jointDef10.lowerAngle = -60*DegToRad ;
		  //jointDef10.upperAngle = 30*DegToRad ;
		  jointDef10.enableMotor = true;
		  jointDef10.maxMotorTorque = 5000;
		  m_world->CreateJoint(&jointDef10); 
		  
		  
		  // right claw
		  b2PolygonShape rightclaw_s;
		  rightclaw_s.SetAsBox(0.5f, 4.0f);	
		  
		  b2FixtureDef rightclaw_fd;
		  rightclaw_fd.shape = &rightclaw_s;
		  rightclaw_fd.density = 0.1f;
		  rightclaw_fd.friction = 100.0f;
		  ball_fd.restitution = 0.0f;
			
		  b2BodyDef rightclaw_bd;
		  rightclaw_bd.type = b2_dynamicBody;
		  rightclaw_bd.position.Set(48.5f , 29.0f); 
		  rightclaw_body = m_world->CreateBody(&rightclaw_bd);
		  rightclaw_body->CreateFixture(&rightclaw_fd);
		  
		  b2RevoluteJointDef jointDef11;
		  jointDef11.bodyA = rightarmsub_body;
		  jointDef11.bodyB = rightclaw_body;
		  jointDef11.localAnchorA.Set(13.5,0);
		  jointDef11.localAnchorB.Set(0.0f,4.0f);
		  jointDef11.collideConnected = false;
		  jointDef11.enableLimit = true;
		  jointDef11.lowerAngle = -30*DegToRad ;
		  jointDef11.upperAngle = 65*DegToRad ;
		  jointDef11.enableMotor = true;
		  jointDef11.maxMotorTorque = 1000;
		  m_world->CreateJoint(&jointDef11);
		  
		  //right spoke
		  b2PolygonShape rightspoke_s;	
		  b2Vec2 verti[4];
		  verti[0].Set(-5,0.5);
		  verti[1].Set(-5.0,-0.5);
		  verti[2].Set(+0.5,-0.5);
		  verti[3].Set(+0.5,0.5);
		  rightspoke_s.Set(verti, 4);
		  
		  
		  b2FixtureDef rightspoke_fd;
		  rightspoke_fd.shape = &rightspoke_s;
		  rightspoke_fd.density = 0.1f;
		  rightspoke_fd.friction = 100.0f;
		  rightspoke_fd.restitution = 0.0f;
			
		  b2BodyDef rightspoke_bd;
		  rightspoke_bd.type = b2_dynamicBody;
		  rightspoke_bd.position.Set(48.5f , 24.5f); 
		  b2Body* rightspoke_body = m_world->CreateBody(&rightspoke_bd);
		  rightspoke_body->CreateFixture(&rightspoke_fd);
		  
		  b2WeldJointDef jointDef12;
		  jointDef12.bodyA = rightclaw_body;
		  jointDef12.bodyB = rightspoke_body;
		  jointDef12.localAnchorA.Set(0.0f,-4.0f); 
		  jointDef12.localAnchorB.Set(0.0f,0.5f);
		  jointDef12.collideConnected = false;
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
		  
		  //sright box's and wall
		  
		  b2PolygonShape box_shape;
		  box_shape.SetAsBox(0.25f, 0.25f); 
			
			
		  b2FixtureDef box_fd;
		  box_fd.shape = &box_shape;
		  box_fd.density = 0.5f;
		  box_fd.friction = 100.0f;
		  box_fd.restitution = 0.0f;
			
		  for (int i = 0; i < 15; i++)
		  {
			  for (int j = 0; j < 15; j++)
			  {
				  
				  b2BodyDef box_bd;
				  box_bd.type = b2_dynamicBody;
				  box_bd.position.Set(60.0f + (i)*2*0.25, 1 + (j)*2*0.25);
				  b2Body* box_b = m_world->CreateBody(&box_bd);
				  box_b->CreateFixture(&box_fd);
			 }
		  }
		  
		  
		  b2PolygonShape wall1_shape;
		  wall1_shape.SetAsBox(1.0f, 6.0f); 
			
			
		  b2FixtureDef wall1_fd;
		  wall1_fd.shape = &wall1_shape;
		  wall1_fd.density = 1000.0f;
		  wall1_fd.friction = 1000.0f;
		  
		  b2BodyDef wall1_bd;
		  wall1_bd.type = b2_dynamicBody;
		  wall1_bd.position.Set(58.5, 5);
		  b2Body* wall1_b = m_world->CreateBody(&wall1_bd);
		  wall1_b->CreateFixture(&wall1_fd);
		  
		  
		  // right extra spoke
		  
		   b2PolygonShape poke1_s;
	      b2Vec2 vertice1[4];
		  vertice1[0].Set(0.5,0.5);
		  vertice1[1].Set(-0.5,1.5);
		  vertice1[2].Set(-0.5,-0.5);
		  vertice1[3].Set(+0.5,-0.5);
		  poke1_s.Set(vertice1, 4);
		  
		  b2FixtureDef poke1_fd;
		  poke1_fd.shape = &poke1_s;
		  poke1_fd.density = 0.01f;
		  poke1_fd.friction = 0.1f;
			
		  b2BodyDef poke1_bd;
		  poke1_bd.type = b2_dynamicBody;
		  poke1_bd.position.Set(-31.5f , 16.5f); 
		  b2Body* poke1_body = m_world->CreateBody(&poke1_bd);
		  poke1_body->CreateFixture(&poke1_fd);
		  
		   b2WeldJointDef jointDef15;
		  jointDef15.bodyA = poke1_body;
		  jointDef15.bodyB = rightspoke_body;
		  jointDef15.localAnchorA.Set(0.5f,0); 
		  jointDef15.localAnchorB.Set(-5,0);
		  jointDef15.collideConnected = false;
		   m_world->CreateJoint(&jointDef15);
		   
		}
   } 
   
  }

  sim_t *sim = new sim_t("Project", dominos_t::create);
}
