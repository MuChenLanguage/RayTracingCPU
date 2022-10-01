#ifndef RECTANGLE_H
#define RECTANGLE_H

#include<glm/glm.hpp>
#include"hittable.h"
#include"Material.h"
#include"Ray.h"

class xy_rect :public hittable {
private:
	float x_min;
	float y_min;
	float x_max;
	float y_max;
	float z = 0;
	std::shared_ptr<Material> mat;
public:
	xy_rect(){}
	xy_rect(float _x_min,float _x_max, float _y_min, float _y_max, float _z,std::shared_ptr<Material> _mat): x_min(_x_min), x_max(_x_max), y_min(_y_min), y_max(_y_max),z(_z),mat(_mat) {
	}

	bool hit(Ray& ray, hit_record& res, float t_min, float t_max) {

		float t = (z - ray.origin()[2]) / ray.direction()[2];
		if (t < t_min || t > t_max) {
			return false;
		}
		glm::vec3 hit_p = ray.get_pos(t);
		if (hit_p[0] < x_min || hit_p[0] > x_max || hit_p[1] < y_min || hit_p[1] > y_max) {
			return false;
		}
		res.hit_p = hit_p;
		res.mat = mat;
		res.normal = glm::dot(glm::vec3(0, 0, 1), ray.direction()) < 0 ? glm::vec3(0, 0, 1) : glm::vec3(0, 0, -1);
		res.t = t;
		return true;
	}
};

class yz_rect :public hittable {
private:
	float y_min;
	float z_min;
	float y_max;
	float z_max;
	float x = 0;
	std::shared_ptr<Material> mat;
public:
	yz_rect() {}
	yz_rect(float _y_min, float _y_max, float _z_min, float _z_max, float _x, std::shared_ptr<Material> _mat) : y_min(_y_min), y_max(_y_max), z_min(_z_min), z_max(_z_max), x(_x), mat(_mat) {
	}

	bool hit(Ray& ray, hit_record& res, float t_min, float t_max) {
		float t = (x - ray.origin()[0]) / ray.direction()[0];
		if (t < t_min || t > t_max) {
			return false;
		}
		glm::vec3 hit_p = ray.get_pos(t);
		if (hit_p[1] < y_min || hit_p[1] > y_max || hit_p[2] < z_min || hit_p[2] > z_max) {
			return false;
		}
		res.hit_p = hit_p;
		res.mat = mat;
		res.normal = glm::dot(glm::vec3(1, 0, 0), ray.direction()) < 0 ? glm::vec3(1, 0, 0) : glm::vec3(-1, 0, 0);
		res.t = t;
		return true;
	}
};

class xz_rect :public hittable {
private:
	float x_min;
	float z_min;
	float x_max;
	float z_max;
	float y = 0;
	std::shared_ptr<Material> mat;
public:
	xz_rect() {}
	xz_rect(float _x_min, float _x_max, float _z_min, float _z_max, float _y, std::shared_ptr<Material> _mat) : x_min(_x_min), x_max(_x_max), z_min(_z_min), z_max(_z_max), y(_y), mat(_mat) {
	}

	bool hit(Ray& ray, hit_record& res, float t_min, float t_max) {
		float t = (y - ray.origin()[1]) / ray.direction()[1];
		if (t < t_min || t > t_max) {
			return false;
		}
		glm::vec3 hit_p = ray.get_pos(t);
		if (hit_p[0] < x_min || hit_p[0] > x_max || hit_p[2] < z_min || hit_p[2] > z_max) {
			return false;
		}
		res.hit_p = hit_p;
		res.mat = mat;
		res.normal = glm::dot(glm::vec3(0, 1, 0), ray.direction()) < 0 ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0);
		res.t = t;
		return true;
	}
};

#endif // !RECTANGLE_H
