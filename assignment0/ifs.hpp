#include<vector>
#include<random>
#include<iostream>

#include "../headers/matrix.hpp"
#include "../headers/vectors.hpp"
#include "../headers/image.hpp"

class Ifs
{
public:
    int n;
    std::vector<Matrix> trans;
    std::vector<double> probs;
public:
    Ifs(){n=0;}
    void add(double prob, const Matrix& m)
    {
        trans.push_back(m);
        probs.push_back(prob);
        n++;
    }
    void draw(int num_points,int num_iters,int size,const char * output_file)
    {

        std::random_device rd1; 
        std::mt19937 gen1(rd1()); 
        std::uniform_real_distribution<> dis1(0, size); 
        std::random_device rd2;  // 用于获取一个随机数种子
        std::mt19937 gen2(rd2()); // 初始化Mersenne Twister随机数生成器
        std::uniform_real_distribution<> dis2(0.0, 1.0); // 定义[0, 1)的均匀分布

        Image image(size,size);
        image.SetAllPixels(Vec3f(1,1,1));
        srand(time(NULL));
        for(int i=0;i<num_points;i++)
        {
            Vec4f p(drand48(),drand48(),0,1.0);
            for(int j=0;j<num_iters;j++)
            {
                int res=0;
                double sum=0.0,sample=dis2(gen2);
                for(int k=0;k<n;k++)
                {
                    sum+=probs[k];
                    if(sample<=sum)
                    {
                        res=k;
                        break;
                    }
                }
                trans[res].Transform(p);
                p/=p.w();
            }
            if(p.x() >= 0 && p.x() < 1 && p.y() >= 0 && p.y() < 1)
                image.SetPixel(p.x()*size,p.y()*size,Vec3f(0,0,0));
        }

        image.SaveTGA(output_file);
    }
};