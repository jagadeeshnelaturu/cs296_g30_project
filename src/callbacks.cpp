/* 
 * Base code for CS 296 Software Systems Lab 
 * Department of Computer Science and Engineering, IIT Bombay
 * Instructor: Parag Chaudhuri
 */

//! These are user defined include files
//! Included in double quotes - the path to find these has to be given at compile time
#include "callbacks.hpp"

#ifndef __APPLE__
#include "GL/glui.h"
#else
#include "GL/glui.h"
#endif

//! The namespace protects the global variables and other names from
//! clashes in scope. Read about the use of named and unnamed
//! namespaces in C++ Figure out where all the datatypes used below
//! are defined
namespace cs296
{
  int32 test_index = 0;
  int32 test_selection = 0;
  int32 test_count = 0;
  cs296::sim_t* entry;
  cs296::base_sim_t* test;
  cs296::settings_t settings;
  int32 width = 640;
  int32 height = 480;
  int32 frame_period = 16;
  int32 main_window;
  float settings_hz = 60.0;
  float32 view_zoom = 2.0f;
  int tx, ty, tw, th;
  bool r_mouse_down;
  b2Vec2 lastp;

  b2Vec2 callbacks_t::convert_screen_to_world(int32 x, int32 y)
  {
    float32 u = x / static_cast<float32>(tw);
    float32 v = (th - y) / float32(th);
    
    float32 ratio = static_cast<float32>(tw) / static_cast<float32>(th);
    b2Vec2 extents(ratio * 25.0f, 25.0f);
    extents *= view_zoom;
    
    b2Vec2 lower = settings.view_center - extents;
    b2Vec2 upper = settings.view_center + extents;
  
    b2Vec2 p;
    p.x = (1.0f - u) * lower.x + u * upper.x;
    p.y = (1.0f - v) * lower.y + v * upper.y;
    return p;
  }
  
  
  void callbacks_t::resize_cb(int32 w, int32 h)
  {
    width = w;
    height = h;
    
    GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
    glViewport(tx, ty, tw, th);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //! Notice the type casting 
    //! Read about explicit/implicit type casting in C++
    float32 ratio = static_cast<float32>(tw) / static_cast<float32>(th);
    
    b2Vec2 extents(ratio * 25.0f, 25.0f);
    extents *= view_zoom;
    
    b2Vec2 lower = settings.view_center - extents;
    b2Vec2 upper = settings.view_center + extents;
  
    //! L/R/B/T extents of the view frustum
    //! Find where this function is defined
    gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
  }
  
  
  void callbacks_t::keyboard_cb(unsigned char key, int x, int y)
  {
    //! What are these?
    B2_NOT_USED(x);
    B2_NOT_USED(y);
    
    switch (key)
    {
    case 27:
      exit(0);
      break;
      
      //! Press 'Z' to zoom out.
    case 'z':
	//! by pressing 'z' once, the world is zoomed out to atmax 1.1 times of the present world 
	/*! max limit on view_zoom is set to 20 */
	/*! then resize the width and height using view_zoom */
      view_zoom = b2Min(1.1f * view_zoom, 20.0f);
      resize_cb(width, height);
      break;
      
    //! Press 'X' to zoom in.
    case 'x':
	//! by pressing 'z' once, the world is zoomed in to atmax 0.9 times of the present world 
	/*! min limit on view_zoom is set to 0.02 */
	/*! then resize the width and height using view_zoom */
      view_zoom = b2Max(0.9f * view_zoom, 0.02f);
      resize_cb(width, height);
      break;
      
    //! Press 'R' to reset.
    case 'r':
	/*! delete all the process that took place from start and initialise test to its start position */
      delete test;
      test = entry->create_fcn();
      break;
      
      //! Press 'P' to pause.
    case 'p':
	/*! set ( pause <= !pause ) */
      settings.pause = !settings.pause;
      break;
      
    case 'd': //! Press 'D' to move right.
	/*! Get the pointer 0f the main body and apply linear impulse to the right by using getbody() and AppluLinearImpulse functions */
	{
	b2Body* temp;
	temp = test->getbody();
	temp->ApplyLinearImpulse(b2Vec2( 30000.0f, 0),temp->GetWorldCenter() , true);
	b2Vec2 vel_body = temp->GetLinearVelocity();
	float speed = vel_body.Normalize();//normalizes vector and returns length
	if ( speed > 25.0f ) 
    		{temp->SetLinearVelocity( 25.0f * vel_body );}
	else {};
	}
      break;

    case 'b': //! Press 'B' to apply break.
	/*! Get the pointer 0f the main body and decrease its velocity to half of its current velocity */
	{
	b2Body* temp2;
	temp2 = test->getbody();
	//temp2->ApplyLinearImpulse(b2Vec2( 30000.0f, 0),temp->GetWorldCenter() , true);
	b2Vec2 vel_body = temp2->GetLinearVelocity();
	temp2->SetLinearVelocity( 0.35f * vel_body );
	}
      break;  
    
    case 'a': //! Press 'A' to  move left.
	/*! Get the pointer 0f the main body and apply linear impulse to the left by using getbody() and AppluLinearImpulse functions */
	{	
	b2Body* temp1;
	temp1 = test->getbody();
	temp1->ApplyLinearImpulse(b2Vec2( -30000.0f, 0),temp1->GetWorldCenter() , true);
	b2Vec2 vel_body = temp1->GetLinearVelocity();
	float speed = vel_body.Normalize();//normalizes vector and returns length
	if ( speed > 25.0f ) 
    		{temp1->SetLinearVelocity( 25.0f * vel_body );}
	else {};
	}
       break;  
    
    case 'w': //! Press 'W' to move left_arm up.
	/*! Get the pointer 0f the left arm and give angular velocity in clockwise direction, negative number,
	* by using getbody() and AppluLinearImpulse functions */
      b2Body* temparm;
      temparm = test->getleftarm();
      temparm->SetAngularVelocity(-1.0f);
	  //key_body1_c->SetAngularDamping(2);
	  //temparm->ApplyLinearImpulse(b2Vec2( 0.0f, 200.0f),temparm->GetWorldCenter() , true);
      break;
      
    case 's': //! Press 'S' to move left_arm down.
	/*! Get the pointer 0f the left arm and give angular velocity in anticlockwise direction, positive number,
	* by using getbody() and AppluLinearImpulse functions */
      b2Body* temparm1;
      temparm1 = test->getleftarm();
      temparm1->SetAngularVelocity(+1.0f);
	  //temparm1->ApplyLinearImpulse(b2Vec2( 0.0f, -200.0f),temparm1->GetWorldCenter() , true);
      break;        
      
    case 'o': //! Press 'O' to move left claw clockwise.
	/*! Get the pointer 0f the left claw and give angular velocity in clockwise direction, negative number,
	* by using getbody() and AppluLinearImpulse functions */
      b2Body* tempclaw;
      tempclaw = test->getclaw();
      tempclaw->SetAngularVelocity(-40.0f);
	  //tempclaw->ApplyLinearImpulse(b2Vec2( 0.0f, +30.0f),tempclaw->GetWorldCenter() , true);
      break;    
      
    case 'l': //! Press 'L' to move left_claw anti-clockwise.
	/*! Get the pointer 0f the left claw and give angular velocity in anticlockwise direction, positive number,
	* by using getbody() and AppluLinearImpulse functions */
      b2Body* tempclaw1;
      tempclaw1 = test->getclaw();
      tempclaw1->SetAngularVelocity(+40.0f);
	  //tempclaw1->ApplyLinearImpulse(b2Vec2( 0.0f, -30.0f),tempclaw1->GetWorldCenter() , true);
      break;     
      
   /* case 'e': // move right_arm up
      b2Body* temprarm;
      temprarm = test->getrightarm();
	  temprarm->ApplyLinearImpulse(b2Vec2( 0.0f, 300.0f),temprarm->GetWorldCenter() , true);
      break;
      
    case 'f': // move right_arm down
      b2Body* temprarm1;
      temprarm1 = test->getrightarm();
	  temprarm1->ApplyLinearImpulse(b2Vec2( 0.0f, -300.0f),temprarm1->GetWorldCenter() , true);
      break; */
      
    case 'e': //! Press 'E' to move rightsub_arm up.
	/*! Get the pointer 0f the right sub_arm and give angular velocity in anticlockwise direction, positive number,
	* by using getbody() and AppluLinearImpulse functions */
      b2Body* temprarmsub;
      temprarmsub = test->getrightarmsub();
      temprarmsub->SetAngularVelocity(+1.0f);
	 // temprarmsub->ApplyLinearImpulse(b2Vec2( 0.0f, 50.0f),temprarmsub->GetWorldCenter() , true);
      break;
      
    case 'f': //! Press 'F' to move rightsub_arm down.
	/*! Get the pointer 0f the right sub_arm and give angular velocity in anticlockwise direction, positive number,
	* by using getbody() and AppluLinearImpulse functions */
      b2Body* temprarmsub1;
      temprarmsub1 = test->getrightarmsub();
      temprarmsub1->SetAngularVelocity(-1.0f);
	  //temprarmsub1->ApplyLinearImpulse(b2Vec2( 0.0f, -50.0f),temprarmsub1->GetWorldCenter() , true);
      break;     
    
    case 'i': //! Press 'I' to move right claw clockwise.
	/*! Get the pointer 0f the right claw and give angular velocity in clockwise direction, negative number,
	* by using getbody() and AppluLinearImpulse functions */
      b2Body* temprclaw;
      temprclaw = test->getrightclaw();
      temprclaw->SetAngularVelocity(-20.0f);
	  //temprclaw->ApplyLinearImpulse(b2Vec2( -30.0f, 30.0f),temprclaw->GetWorldCenter() , true);
      break;        
      
    case 'k': //! Press 'K' to move right claw anti-clockwise.
	/*! Get the pointer 0f the right claw and give angular velocity in anticlockwise direction, positive number,
	* by using getbody() and AppluLinearImpulse functions */
      b2Body* temprclaw1;
      temprclaw1 = test->getrightclaw();
      temprclaw1->SetAngularVelocity(+20.0f);
	  //temprclaw1->ApplyLinearImpulse(b2Vec2( 30.0f, -30.0f),temprclaw1->GetWorldCenter() , true);
      break;      
      
      //! The default case. Why is this needed?
    default:
      if (test)
	{
	  test->keyboard(key);
	}
    }
  }
  
  
  void callbacks_t::keyboard_special_cb(int key, int x, int y)
  {
    B2_NOT_USED(x);
    B2_NOT_USED(y);
    
    switch (key)
    {
    case GLUT_ACTIVE_SHIFT:
      
      //! Press 'left arrow' to pan left.
    case GLUT_KEY_LEFT:
      settings.view_center.x -= 0.5f;
      resize_cb(width, height);
      break;
      
    //! Press 'right arrow' to pan right.
    case GLUT_KEY_RIGHT:
      settings.view_center.x += 0.5f;
      resize_cb(width, height);
      break;
      
    //! Press 'down arrow' to pan down.
    case GLUT_KEY_DOWN:
      settings.view_center.y -= 0.5f;
      resize_cb(width, height);
      break;
      
    //! Press 'up arrow' to pan up.
    case GLUT_KEY_UP:
      settings.view_center.y += 0.5f;
      resize_cb(width, height);
      break;
      
    //! Press home to reset the view.
    case GLUT_KEY_HOME:
      view_zoom = 1.0f;
      settings.view_center.Set(0.0f, 20.0f);
      callbacks_t::resize_cb(width, height);
      break;
    }
  }

  void callbacks_t::keyboard_up_cb(unsigned char key, int x, int y)
  {
    B2_NOT_USED(x);
    B2_NOT_USED(y);
    
    if (test)
      {
	test->keyboard_up(key);
      }
  }
  
  void callbacks_t::mouse_cb(int32 button, int32 state, int32 x, int32 y)
  {
    //! Use the mouse to move things around - figure out how this works?
    if (button == GLUT_LEFT_BUTTON)
      {
	int mod = glutGetModifiers();
	b2Vec2 p = convert_screen_to_world(x, y);
	if (state == GLUT_DOWN)
	  {
	    b2Vec2 p = convert_screen_to_world(x, y);
	    if (mod == GLUT_ACTIVE_SHIFT)
	      {
		test->shift_mouse_down(p);
	      }
	    else
	      {
		test->mouse_down(p);
	      }
	  }
	
	if (state == GLUT_UP)
	  {
	    test->mouse_up(p);
	  }
      }
    else if (button == GLUT_RIGHT_BUTTON)
      {
	if (state == GLUT_DOWN)
	  {	
	    lastp = convert_screen_to_world(x, y);
	    r_mouse_down = true;
	  }
	
	if (state == GLUT_UP)
	  {
	  r_mouse_down = false;
	  }
      }
  }
  
  
  void callbacks_t::mouse_motion_cb(int32 x, int32 y)
  {
    b2Vec2 p = convert_screen_to_world(x, y);
    test->mouse_move(p);
    
    if (r_mouse_down)
      {
	b2Vec2 diff = p - lastp;
	settings.view_center.x -= diff.x;
	settings.view_center.y -= diff.y;
	resize_cb(width, height);
	lastp = convert_screen_to_world(x, y);
      }
  }
  
  void callbacks_t::timer_cb(int)
  {
    glutSetWindow(main_window);
    glutPostRedisplay();
    glutTimerFunc(frame_period, timer_cb, 0);
  }
  
  void callbacks_t::display_cb(void)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    test->set_text_line(30);
    b2Vec2 old_center = settings.view_center;
    settings.hz = settings_hz;
    
    test->step(&settings);
    
    if (old_center.x != settings.view_center.x || old_center.y != settings.view_center.y)
      {
	resize_cb(width, height);
      }
    
    test->draw_title(5, 15, entry->name);
    
    glutSwapBuffers();
    
    if (test_selection != test_index)
      {
	test_index = test_selection;
	delete test;
	entry = cs296::sim;
	test = entry->create_fcn();
	view_zoom = 1.0f;
	settings.view_center.Set(0.0f, 20.0f);
      resize_cb(width, height);
      }
  }
  
  
  
  void callbacks_t::restart_cb(int)
  {
    delete test;
    entry = cs296::sim;
    test = entry->create_fcn();
    resize_cb(width, height);
  }
  
  void callbacks_t::pause_cb(int)
  {
    settings.pause = !settings.pause;
  }
  
  void callbacks_t::exit_cb(int code)
  {
    exit(code);
  }
  
  void callbacks_t::single_step_cb(int)
  {
    settings.pause = 1;
    settings.single_step = 1;
  }

};
