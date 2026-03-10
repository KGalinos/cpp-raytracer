/**
 * @file Assignment2.cpp
 *  
 * @brief Assignment 2 - HY-150
 * 
 * @author: csd5582
 */

#include <iostream>
#include <fstream>
#include "std_lib_facilities.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"



class Sphere {        
  
  public:         
  vec3 center;
  double radius; 
  vec3 colour;
  
   
    Sphere(const vec3& c, float r, const vec3& col) : center(c), radius(r), colour(col) {}

    void setCenter(const vec3& c) {
        center = c;
    }

    void setRadius(float r) {
        radius = r;
    }

    void setColour(const vec3& col) {
        colour = col;
    }

    vec3 getCenter() const {
        return center;
    }

    double getRadius() const {
        return radius;
    }

    vec3 getColour() const {
        return colour;
    }
    
    bool hit(const ray& r, double& distance) {
        vec3 oc = r.origin() - center;
        auto a = dot(r.direction(), r.direction());
        auto b = 2.0 * dot(oc, r.direction());  
        auto c = dot(oc, oc) - radius * radius;
        auto discriminant = b * b - 4 * a * c;

        //calculates diakrinousa to solve equation betweeb ray and sphere
        if (discriminant > 0) {
            auto temp = (-b - sqrt(discriminant)) / (2 *a);
            if (temp < distance && temp > 0) {
                distance = temp;
                return true;
            }
            temp = (-b + sqrt(discriminant)) / (2 * a);
            if (temp < distance && temp > 0) {
                distance = temp;
                return true;
            }
        }

        return false;
    } 

    double dist(const ray& r){
        
        vec3 oc = r.origin() - center;
        auto a = dot(r.direction(), r.direction());
        auto b = 2.0 * dot(oc, r.direction());
        auto c = dot(oc, oc) - radius * radius;
        auto discriminant = b * b - 4 * a * c;
        //finds x1,x2 of diakrinousa and sends the smallest which is the distance.
        if (discriminant > 0) {
            auto temp1 = (-b - sqrt(discriminant)) / (2 *a);
            auto temp2 = (-b + sqrt(discriminant)) / (2 * a);
            if ( temp1 > temp2) {
                return temp2;
            }else{
                return temp1;
            }
        }
    }

};

class Camera {       
    public:          
    vec3 center;
    int width;
    int height;
    Camera() : center(vec3(0.0, 0.0, 0.0)), width(0), height(0) {}
    
    Camera(const vec3& c, int w, int h) : center(c), width(w), height(h) {}

    void setCenter(const vec3& c) {
        center = c;
    }

    void setWidth(int w) {
        width = w;
    }

    void setHeight(int h) {
        height = h;
    }

    vec3 getCenter() const {
        return center;
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }


    vec3 GetPixelCenter(int i, int j){
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(width) / height);
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);
        auto pixel_delta_u = viewport_u / width;
        auto pixel_delta_v = viewport_v / height;
        auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        return pixel_center;
    }
    
};


class World {       
  public:             
    vector<Sphere> Spheres;
    vec3 SkyColour;
    color colour;
    string name;
    int spherecount;

    void Add(Sphere& sphere) {
            Spheres.emplace_back(sphere);
    }
    
void Render(Camera& camera1) {
    ofstream fout;          //opens a file with 
    name = name + ".ppm";
    fout.open(name);
    fout << "P3\n" << camera1.width << ' ' << camera1.height << "\n255\n";
    int temp = -1;
    int prec;

    for (int j = 0; j < camera1.height; ++j) {
        for (int i = 0; i < camera1.width; ++i) {
            //calculates precentage and makes sure not to print twice
            double tt = static_cast<double>(j) / (camera1.height - 1);
            prec = tt * 100;
            if (prec != temp) {
                cout << prec << "%" " completed\n";             
                temp = prec;
            }
            temp = prec;
            
            //makes the ray
            vec3 pixel_center = camera1.GetPixelCenter(i, j);
            auto ray_direction = pixel_center - camera1.center;
            ray r(camera1.center, ray_direction);
            color colour = SkyColour;
            
            //for each pixel check if it hit any sphere if it did check if its the closest one 
            //if it is colour of sphere if it didnt hit colour of sky
            double min_distance = infinity;
            for (auto& sphere : Spheres) {
                if (sphere.hit(r, min_distance)) {        
    
                        min_distance = sphere.dist(r);
                        colour = sphere.getColour();
                    }
                }
            write_color(fout, colour);
        }
    }
    fout.close();
}




};




int main()
{
    ifstream infile("world.txt");
    try
    {
        if (!infile.is_open()) {
            throw runtime_error("Error opening file");
        }

        World world;
        Camera camera1;
        string line;
        
        //readlines
        string name;
        getline(infile, name);
        world.name = name;
        //readslines and also different metavlites that have space bettween
        
        getline(infile, line);
            istringstream iss(line);
            float c_x, c_y, c_z;
            iss >> c_x >> c_y >> c_z;
            camera1.setCenter(vec3(c_x, c_y, c_z));

        getline(infile, line);
            istringstream(line)>>camera1.width;
        
        getline(infile, line);
            istringstream(line)>>camera1.height;
    
    {
        getline(infile, line);
        istringstream iss(line);
        double bg_r, bg_g, bg_b;
        bg_r = bg_g = bg_b = 0.0;
        iss >> bg_r >> bg_g >> bg_b;
        world.SkyColour = vec3(bg_r, bg_g, bg_b);
        
    }
        
        

        int sphereCount;
        getline(infile, line);
        istringstream(line) >> sphereCount;
        world.spherecount = sphereCount;
        
        vec3 a;
        vec3 b;
        for (int i = 0; i < sphereCount; ++i) {
            getline(infile, line);
            istringstream iss(line);
            double ax, ay, az, bx, by, bz, radius;
            iss >> ax >> ay >> az >> bx >> by >> bz >> radius;
            
            //cout<<ax<<" "<<ay<<" "<<az<<" "<<bx<<" "<<by<<" "<<bz<<" "<<radius<<" "<<"\n";

            vec3 a(ax, ay, az);
            vec3 b(bx, by, bz);
            Sphere sphere1(b, radius, a);
            world.Add(sphere1);
        }

        world.Render(camera1);

        infile.close();

    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        cin.clear();
        return 1;
    }

    return 0;
}