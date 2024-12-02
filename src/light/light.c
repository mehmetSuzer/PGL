
#include "light.h"

static void assert_color(vec3f color) {
    assert(0.0f <= color.r && color.r <= 1.0f && 
           0.0f <= color.g && color.g <= 1.0f && 
           0.0f <= color.b && color.b <= 1.0f);
}

static void assert_attenuation_coefs(f32 quadratic, f32 linear) {
    assert(0.0f <= linear && 0.0f <= quadratic);
}

static void assert_intensity(f32 intensity) {
    assert(0.0f < intensity && intensity <= 1.0f);
}

static void assert_direction(vec3f direction) {
    assert(epsilon_equal(vec3f_mag2(direction), 1.0f, PGLM_EPSILON));
}

static void assert_cutoff_angle(f32 cutoff_angle) {
    assert(0.0f < cutoff_angle && cutoff_angle < PGLM_PIf);
}

static void calculate_attenuation_coefs(f32 distance1, f32 attenuation1, f32 distance2, f32 attenuation2, f32* quadratic, f32* linear) {
    const f32 d1_minus_d2 = distance1 - distance2;
    const f32 a1_d1 = attenuation1 * distance1;
    const f32 a2_d2 = attenuation2 * distance2;
    const f32 d1_d2 = distance1 * distance2;

    *quadratic = 1.0f / (a1_d1 * d1_minus_d2) - 1.0f / (a2_d2 * d1_minus_d2) + 1.0f / d1_d2;
    *linear = (1.0f - attenuation1 - a1_d1 * distance1 * (*quadratic)) / a1_d1;
}

// ------------------------------------------------------------------------------------------------- //

point_light_t point_light_init(vec3f color, vec3f position, f32 distance1, f32 attenuation1, f32 distance2, f32 attenuation2) {
    f32 quadratic;
    f32 linear;
    calculate_attenuation_coefs(distance1, attenuation1, distance2, attenuation2, &quadratic, &linear);
    assert_attenuation_coefs(quadratic, linear);
    assert_color(color);

    return (point_light_t){
        .color = color,
        .position = position,
        .quadratic = quadratic,
        .linear = linear,
    };
}

f32 point_light_attenuation(point_light_t light, vec3f point) {
    const f32 distance = vec3f_mag(vec3f_sub(point, light.position));
    const f32 attenuation = 1.0f / ((light.quadratic * distance + light.linear) * distance + 1.0f);
    return attenuation;
}

// ------------------------------------------------------------------------------------------------- //

directional_light_t directional_light_init(vec3f color, vec3f direction, f32 intensity) {
    assert_intensity(intensity);
    assert_direction(direction);
    assert_color(color);

    return (directional_light_t){
        .color = color, 
        .direction = direction,
        .intensity = intensity,
    };
}

// ------------------------------------------------------------------------------------------------- //

spot_light_t spot_light_init(vec3f color, vec3f position, vec3f direction, f32 distance1, f32 attenuation1, f32 distance2, f32 attenuation2, f32 cutoff_angle) {
    f32 quadratic;
    f32 linear;
    calculate_attenuation_coefs(distance1, attenuation1, distance2, attenuation2, &quadratic, &linear);
    assert_attenuation_coefs(quadratic, linear);
    assert_cutoff_angle(cutoff_angle);
    assert_direction(direction);
    assert_color(color);

    return (spot_light_t){
        .color = color,
        .position = position,
        .direction = direction,
        .quadratic = quadratic,
        .linear = linear,
        .cos_cutoff_angle = cosf(cutoff_angle),
    };
}

f32 spot_light_attenuation(spot_light_t light, vec3f point) {
    const vec3f light_to_point = vec3f_sub(point, light.position);
    const f32 distance = vec3f_mag(light_to_point);
    const vec3f direction_to_point = vec3f_scale(light_to_point, 1.0f / distance);
    const f32 cos_angle_between_directions = vec3f_dot(direction_to_point, light.direction);
    
    if (cos_angle_between_directions < light.cos_cutoff_angle) {
        return 0.0f;
    }

    const f32 attenuation = 1.0f / ((light.quadratic * distance + light.linear) * distance + 1.0f);
    return attenuation;
}
