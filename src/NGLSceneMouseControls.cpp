#include "NGLScene.h"
#include <QMouseEvent>
#include <ngl/Util.h>

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent( QMouseEvent* _event )
{
  // note the method buttons() is the button state when event was called
  // that is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if ( m_win.rotate && _event->buttons() == Qt::LeftButton )
  {
    int diffx = _event->x() - m_win.origX;
    int diffy = _event->y() - m_win.origY;
    m_win.spinXFace += static_cast<int>( 0.5f * diffy );
    m_win.spinYFace += static_cast<int>( 0.5f * diffx );
    m_win.origX = _event->x();
    m_win.origY = _event->y();
    update();
  }
  // right mouse translate code
  else if ( m_win.translate && _event->buttons() == Qt::RightButton )
  {
    int diffX      = static_cast<int>( _event->x() - m_win.origXPos );
    int diffY      = static_cast<int>( _event->y() - m_win.origYPos );
    m_win.origXPos = _event->x();
    m_win.origYPos = _event->y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    update();
  }
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent( QMouseEvent* _event )
{
  // that method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if ( _event->button() == Qt::LeftButton )
  {
    m_win.origX  = _event->x();
    m_win.origY  = _event->y();
    m_win.rotate = true;


//    ngl::Vec3 nearsource(_event->x(), _event->y(), 0.0f);
//    ngl::Vec3 farsource(_event->x(),_event->y(), 1.0f);


//    nearPoint = ngl::unProject(nearsource,
//                                         M,
//                                         m_cam.getProjectionMatrix(),
//                                         ngl::Vec4(0,0,width(),height()));

//    ngl::Vec3 farPoint = ngl::unProject(farsource,
//                                         M,
//                                         m_cam.getProjectionMatrix(),
//                                         ngl::Vec4(0,0,width(),height()));

//        // Create a ray from the near clip plane to the far clip plane.
//        direction = farPoint - nearPoint;
//        direction.normalize();
////        Ray pickRay = new Ray(nearPoint, direction);


//        hit=raySphere(nearPoint, direction, m_spherePos, m_sphereRadius);

//    if(hit)
//    {
//        std::cout<<"Hit"<<'\n';
//    }
//    else
//    {
//        std::cout<<"Not Hit"<<'\n';
//    }

    //////////////////////////////////////////////


    float x = (2.0f * _event->x()) / width() - 1.0f;
    float y = 1.0f - (2.0f * _event->y()) / height();
    float z = 1.0f;
    ngl::Vec3 ray_nds(x, y, z);
    ngl::Vec4 ray_clip(ray_nds.m_x, ray_nds.m_y, -1.0, 1.0);
    ngl::Mat4 proj=m_cam.getProjectionMatrix();
    ngl::Vec4 ray_eye = (proj.inverse()) * ray_clip;
    ray_eye.set(ray_eye.m_x, ray_eye.m_y, -1.0, 0.0);

    ngl::Vec4 ray_wor( proj * ray_eye.toVec3() );
    // don't forget to normalise the vector at some point

    ngl::Vec3 ray_worv3=ray_wor.toVec3();


    hit=raySphere(ray_eye, ray_worv3, m_spherePos, m_sphereRadius);

    std::cout<<"nearPoint="<<nearPoint<<'\n';
    std::cout<<"direction="<<direction<<'\n';

    if(hit)
    {
        std::cout<<"Hit"<<'\n';
    }
    else
    {
        std::cout<<"Not Hit"<<'\n';
    }



  }
  // right mouse translate mode
  else if ( _event->button() == Qt::RightButton )
  {
    m_win.origXPos  = _event->x();
    m_win.origYPos  = _event->y();
    m_win.translate = true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent( QMouseEvent* _event )
{
  // that event is called when the mouse button is released
  // we then set Rotate to false
  if ( _event->button() == Qt::LeftButton )
  {
    m_win.rotate = false;
  }
  // right mouse translate mode
  if ( _event->button() == Qt::RightButton )
  {
    m_win.translate = false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent( QWheelEvent* _event )
{

  // check the diff of the wheel position (0 means no change)
  if ( _event->delta() > 0 )
  {
    m_modelPos.m_z += ZOOM;
  }
  else if ( _event->delta() < 0 )
  {
    m_modelPos.m_z -= ZOOM;
  }
  update();
}
