/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef FRAME_H
#define FRAME_H

#include "myslam/common_include.h"
#include "myslam/camera.h"

namespace myslam
{

class Frame
{
public:
    typedef std::shared_ptr<Frame> Ptr;
    unsigned long                  id_;         // id of this frame
    double                         time_stamp_; // when it is recorded
    SE3                            T_c_w_;      // transform from world to camera
    Camera::Ptr                    camera_;     // Pinhole RGBD Camera model
    std::shared_ptr<Mat>           color_, depth_; // color and depth image
    bool                           is_key_frame_;  // whether a key-frame

public: // data members
    Frame();
    Frame(long id);

    static Frame::Ptr createFrame(std::shared_ptr<Mat> color, std::shared_ptr<Mat> depth, double time_stamp);

    // find the depth in depth map
    double findDepth( const cv::KeyPoint& kp );

    // Get Camera Center
    Vector3d getCamCenter() const;

    void setPose( const SE3& T_c_w );

    // check if a point is in this frame
    bool isInFrame( const Vector3d& pt_world );
};

}

#endif // FRAME_H
