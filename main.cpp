#include "include/Scene.hpp"

using namespace std;

int main(int argc, char** argv){
    Scene* scene = new Scene(argc, argv);
    scene->run();
    return 0;
}