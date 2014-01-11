#ifndef _RIDGELINETOOLS_
#define _RIDGELINETOOLS_

void findMaxima (std::vector<std::vector<float> >,
        std::vector<std::vector<float> >);

void buildRidgeLine (std::vector<ridgePoint>, const
        std::vector<std::vector<float> >);

void getRidgeLines (std::vector<std::vector<ridgePoint> >, const
        std::vector<std::vector<float> >);

#endif
