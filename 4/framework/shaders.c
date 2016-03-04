/* Computer Graphics and Game Technology, Assignment Ray-tracing
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "shaders.h"
#include "perlin.h"
#include "v3math.h"
#include "intersection.h"
#include "scene.h"
#include "quat.h"
#include "constants.h"

// shade_constant()
//
// Always return the same color. This shader does no real computations
// based on normal, light position, etc. As such, it merely creates
// a "silhouette" of an object.

vec3
shade_constant(intersection_point ip)
{
    return v3_create(1, 0, 0);
}


vec3
shade_matte(intersection_point ip)
{
    float contr;
    vec3 light_source;
    vec3 pixel_light = v3_create(0, 0, 0);
    vec3 ones  = v3_create(1, 1, 1);
    vec3 offset = v3_multiply(ones, 0.0001);
    if (scene_ambient_light > 0) {
        pixel_light = v3_add(pixel_light, v3_multiply(ones, scene_ambient_light));
    }
    for (int i = 0; i < scene_num_lights; i++)
    {
        light_source = v3_normalize(v3_add(scene_lights[i].position, ip.p));
        if (shadow_check(v3_add(ip.p, offset), light_source)) {
            contr = 0;
        } else {
            contr = scene_lights[i].intensity * v3_dotprod(light_source, ip.n);
            if (contr < 0) contr = 0;
        }
        pixel_light = v3_add(pixel_light, v3_multiply(ones, contr));
    }
    return pixel_light;
}


vec3
shade_blinn_phong(intersection_point ip)
{
    // cf: final color, cd: diffuse-reflexion-coefficient, cs: shinyness
    // Ia: intensity of ambient light, Ii: intensity of light source
    // li: direction of light source, n: normal, h: halfway vector
    // kd: diffuse contribution, ks: specular contribution, a: phong exponent
    vec3 ones = v3_create(1,1,1), h, diff, spec, cd = v3_create(1,0,0), cs = ones;
    float phong, dot, sources = 0, reflections = 0, kd = 0.8, ks = 0.5, a = 50;
    
    for (int i = 0; i < scene_num_lights; i++) {
        // sources = sum(Ii * max(0, n.li))
        dot = v3_dotprod(ip.n, scene_lights[i].position);
        if (dot < 0) dot = 0;
        sources += scene_lights[i].intensity * dot;
        // reflections = sum(Ii * (n.h)^a)
        h = v3_multiply(v3_add(ip.i, ones), 1/v3_length(v3_add(ip.i, ones)));
        phong = pow(v3_dotprod(ip.n, h), a);
        reflections += scene_lights[i].intensity * phong;
    }
    // cf = cd * (Ia + (kd * sources)) + cs * ks * reflections
    diff = v3_multiply(cd, (scene_ambient_light + kd * sources));
    spec = v3_multiply(cs, (ks * reflections));
    return v3_add(diff, spec);
}

vec3
shade_reflection(intersection_point ip)
{
    return v3_create(1, 0, 0);
}

// Returns the shaded color for the given point to shade.
// Calls the relevant shading function based on the material index.
vec3
shade(intersection_point ip)
{
  switch (ip.material)
  {
    case 0:
      return shade_constant(ip);
    case 1:
      return shade_matte(ip);
    case 2:
      return shade_blinn_phong(ip);
    case 3:
      return shade_reflection(ip);
    default:
      return shade_constant(ip);

  }
}

// Determine the surface color for the first object intersected by
// the given ray, or return the scene background color when no
// intersection is found
vec3
ray_color(int level, vec3 ray_origin, vec3 ray_direction)
{
    intersection_point  ip;

    // If this ray has been reflected too many times, simply
    // return the background color.
    if (level >= 3)
        return scene_background_color;

    // Check if the ray intersects anything in the scene
    if (find_first_intersection(&ip, ray_origin, ray_direction))
    {
        // Shade the found intersection point
        ip.ray_level = level;
        return shade(ip);
    }

    // Nothing was hit, return background color
    return scene_background_color;
}
