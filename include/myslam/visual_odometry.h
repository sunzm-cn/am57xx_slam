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

#ifndef VISUALODOMETRY_H
#define VISUALODOMETRY_H

#include <opencv2/features2d/features2d.hpp>

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/custom_reaction.hpp>

#include "myslam/common_include.h"
#include "myslam/map.h"
#include "myslam/frame.h"

namespace myslam
{

class VisualOdometry
{
public:
    typedef shared_ptr<VisualOdometry> Ptr;

    Map::Ptr    map_;       // map with all frames and map points
    Frame::Ptr  ref_;       // reference key-frame
    Frame::Ptr  curr_;      // current frame

    cv::Ptr<cv::ORB> orb_;  // orb detector and computer
    vector<cv::KeyPoint>    keypoints_curr_;    // keypoints in current frame
    Mat                     descriptors_curr_;  // descriptor in current frame

    cv::FlannBasedMatcher   matcher_flann_;     // flann matcher
    vector<MapPoint::Ptr>   match_3dpts_;       // matched 3d points
    vector<int>             match_2dkp_index_;  // matched 2d pixels (index of kp_curr)

    SE3 T_c_w_estimated_;    // the estimated pose of current frame
    int num_inliers_;        // number of inlier features in icp
    int num_lost_;           // number of lost times

    // parameters
    int num_of_features_;   // number of features
    double scale_factor_;   // scale in image pyramid
    int level_pyramid_;     // number of pyramid levels
    float match_ratio_;     // ratio for selecting  good matches
    int max_num_lost_;      // max number of continuous lost times
    int min_inliers_;       // minimum inliers
    double key_frame_min_rot;   // minimal rotation of two key-frames
    double key_frame_min_trans; // minimal translation of two key-frames
    double  map_point_erase_ratio_; // remove map point ratio

public: // functions
    VisualOdometry(const std::string& filename);
    ~VisualOdometry() {};

    bool addFrame( Frame::Ptr frame );      // add a new frame
    bool addFirstFrame( Frame::Ptr frame );      // add a new frame
    bool addNextFrame( Frame::Ptr frame );      // add a new frame

protected:
    // inner operation
    void extractKeyPoints();
    void computeDescriptors();
    void featureMatching();
    void poseEstimationPnP();
    void optimizeMap();

    void addKeyFrame();
    void addMapPoints();
    bool checkEstimatedPose();
    bool checkKeyFrame();

    double getViewAngle( Frame::Ptr frame, MapPoint::Ptr point );

};

namespace sc = boost::statechart;

struct EvAddFrame : sc::event<EvAddFrame>
{
public:
    EvAddFrame(Frame::Ptr frame) : frame_(frame) {};

    Frame::Ptr get_frame() const { return frame_; };

private:
    Frame::Ptr frame_;
};

struct Init;
struct VO : sc::state_machine<VO, Init>, VisualOdometry
{
public:
    VO(const std::string& filename) : VisualOdometry(filename) {};

    string get_dataset_dir();
    bool is_lost() { return lost_flag_; };
    void set_lost() { lost_flag_ = true; };

private:
    bool lost_flag_ = false;
};

struct Init : sc::simple_state<Init, VO>
{
    typedef sc::custom_reaction<EvAddFrame> reactions;
    sc::result react(const EvAddFrame &);
};

struct Ok : sc::simple_state<Ok, VO>
{
    typedef sc::custom_reaction<EvAddFrame> reactions;
    sc::result react(const EvAddFrame &);
};

struct Lost : sc::simple_state<Lost, VO>
{
    typedef sc::custom_reaction<EvAddFrame> reactions;
    sc::result react(const EvAddFrame &);
};

}

#endif // VISUALODOMETRY_H
