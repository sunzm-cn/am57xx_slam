// -------------- test the visual odometry -------------
#include <fstream>
#include <boost/timer.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "myslam/visual_odometry.h"

bool get_rgbd_files(const string & dataset_dir, vector<string> & rgb_files, vector<string> & depth_files, vector<double> & rgb_times)
{
    cout << "dataset: " << dataset_dir << endl;
    ifstream fin (dataset_dir+"/associate.txt");
    if (!fin)
    {
        cout << "please generate the associate file called associate.txt!" << endl;
        return false;
    }

    vector<double> depth_times;
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
    return true;
}

bool get_rgbd_data(const string & rgb_file, const string & depth_file, std::shared_ptr<Mat> & color, std::shared_ptr<Mat> & depth)
{
    color = std::make_shared<Mat>(cv::imread (rgb_file));
    depth = std::make_shared<Mat>(cv::imread (depth_file, -1));
    if (color->data==nullptr || depth->data==nullptr)
        return false;
    return true;
}

int main ( int argc, char** argv )
{
    if ( argc != 3 )
    {
        cout<<"usage: run_vo parameter_file dataset_directory"<<endl;
        return 1;
    }
    myslam::VO vo(argv[1]);
    vo.initiate();

    vector<string> rgb_files, depth_files;
    vector<double> rgb_times;
    if (!get_rgbd_files(string(argv[2]), rgb_files, depth_files, rgb_times))
        return 1;

    for (int i=0; i<rgb_files.size(); i++)
    {
        cout<<"****** loop "<<i<<" ******"<<endl;

        std::shared_ptr<Mat> color, depth;
        if (!get_rgbd_data(rgb_files[i], depth_files[i], color, depth))
            break;

        boost::timer timer;
        vo.add_frame(color, depth, rgb_times[i]);
        cout << "VO costs time: " << timer.elapsed() << endl;

        if(vo.is_lost())
            break;
        cout << endl;
    }

    return 0;
}
