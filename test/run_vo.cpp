// -------------- test the visual odometry -------------
#include <fstream>
#include <boost/timer.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "myslam/visual_odometry.h"

int main ( int argc, char** argv )
{
    if ( argc != 2 )
    {
        cout<<"usage: run_vo parameter_file"<<endl;
        return 1;
    }
    myslam::VO vo(argv[1]);
    vo.initiate();

    string dataset_dir = vo.get_dataset_dir();
    cout<<"dataset: "<<dataset_dir<<endl;
    ifstream fin (dataset_dir+"/associate.txt");
    if (!fin)
    {
        cout<<"please generate the associate file called associate.txt!"<<endl;
        return 1;
    }

    vector<string> rgb_files, depth_files;
    vector<double> rgb_times, depth_times;
    while (!fin.eof())
    {
        string rgb_time, rgb_file, depth_time, depth_file;
        fin>>rgb_time>>rgb_file>>depth_time>>depth_file;
        rgb_times.push_back(atof(rgb_time.c_str()));
        depth_times.push_back(atof(depth_time.c_str()));
        rgb_files.push_back(dataset_dir + "/" + rgb_file);
        depth_files.push_back(dataset_dir + "/" + depth_file);

        if (fin.good() == false)
            break;
    }

    cout<<"read total "<<rgb_files.size() <<" entries"<<endl;
    for (int i=0; i<rgb_files.size(); i++)
    {
        cout<<"****** loop "<<i<<" ******"<<endl;
        Mat color = cv::imread (rgb_files[i]);
        Mat depth = cv::imread (depth_files[i], -1);
        if (color.data==nullptr || depth.data==nullptr)
            break;
        boost::timer timer;
        vo.process_event(myslam::EvAddFrame(myslam::Frame::createFrame(std::make_shared<Mat>(color), std::make_shared<Mat>(depth), rgb_times[i])));
        cout << "VO costs time: " << timer.elapsed() << endl;

        if(vo.is_lost())
            break;
        cout << endl;
    }

    return 0;
}
