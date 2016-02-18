#include <random>
#include <iostream>
#include <string>


#if defined(_OUTPUT_RAW_SAMPLES_2_FILE_)
#include "stdio.h"
#endif

//simulate a gaussian distribution source at N(m,s), 
//where the sample 

void main()
{
    const int fs = 53e4; 
    const int M   = fs*1e-3;   //1ms of samples
    const int Nt  = 10000; 
    const int N   = M*Nt;    //1 sec of samples

    double m = 0.0; 
    double s = 20.0; 
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(m,s);

    double dAGC_threshold = 0; 

#if defined(_OUTPUT_RAW_SAMPLES_2_FILE_)
    FILE *fp;
    errno_t err = fopen_s(&fp,"normal_distrib.txt","wt");
    if(err != 0)
    {
        printf("cannot open numericalODE.txt,exit");
        exit(0); 
    }
#endif

    const int Level = 6; 
    int p[Level] = {}; 
    int R        = 0; 
    double percent_higher = 0; 
    for(int ms = 0; ms < Nt; ms++)
    {
        int count = 0; 
        for(int i = 0; i < M; i++)
        {
            double number = distribution(generator);

#if defined(_OUTPUT_RAW_SAMPLES_2_FILE_)
            fprintf(fp,"%.3f\n",number); 
#endif

            if(fabs(number)> 2*dAGC_threshold)
            {
                count++; 
            }

            if(number> 2*dAGC_threshold)
            {
                p[5]++; 
            }
            else if(number > dAGC_threshold)
            {
                p[4]++;     
            }
            else if(number > 0 )
            {
                p[3]++;
            }
            else if(number > -dAGC_threshold)
            {
                p[2]++;
            }
            else if(number > -2*dAGC_threshold)
            {
                p[1]++;
            }            
            else
            {
                p[0]++;
            }
        }

        R += M; 
        percent_higher = count/(double)M; 

        dAGC_threshold += (percent_higher - 0.25 )/5; 

        printf("percentage higher %.3f with threshold %.3f   %.3f  %.3f  %.3f  %.3f  %.3f  %.3f\n",percent_higher,dAGC_threshold, p[0]/(double)R,
            p[1]/(double)R,p[2]/(double)R,p[3]/(double)R,p[4]/(double)R,p[5]/(double)R); 
    }

     for (int i=0; i<Level; ++i)
     {
         std::cout << i << "-" << (i+1) << ": ";
         std::cout << std::string(p[i]*250/R,'*') << std::endl;
     }

#if defined(_OUTPUT_RAW_SAMPLES_2_FILE_)
    fclose(fp); 
#endif

}
