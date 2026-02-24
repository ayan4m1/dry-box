include <BOSL2/std.scad>
include <BOSL2/rounding.scad>

hole_diameter = 7;
cable_diameter = 5.6;
gasket_diameter_padding = 4;
gasket_diameter = hole_diameter + (gasket_diameter_padding * 2);
box_wall_thickness = 2;
gasket_wall_thickness = 1.6;
strain_relief_wall_thickness = 1.6;
strain_relief_height = 8;
strain_relief_diameter = cable_diameter + (strain_relief_wall_thickness * 2);

gasket_bottom_height = box_wall_thickness + (gasket_wall_thickness * 2);
gasket_height = gasket_bottom_height + strain_relief_height;

$fn = 32;

module ring(d_outer, d_inner, height) {
    difference() {
        cylinder(d = d_outer, h = height);
        translate([0, 0, (height / 2) - 0.1]) offset_sweep(circle(d = d_inner), height = height + 0.2, top = os_circle(r = -height / 2), bottom = os_circle(r = -height / 2));
    }
}

difference() {
union() {
translate([0, 0, gasket_bottom_height / 2]) offset_sweep(circle(d = gasket_diameter), height = gasket_bottom_height, top = os_smooth(joint = 1));
translate([0, 0, gasket_bottom_height + (strain_relief_height / 2)]) offset_sweep(circle(d = strain_relief_diameter), height = strain_relief_height, top = os_smooth(joint = 2), bottom = os_circle(r = -gasket_diameter_padding / 2));
}

translate([0, 0, gasket_height]) rounding_hole_mask(d = cable_diameter, rounding = 1);

translate([0, 0, (gasket_bottom_height - box_wall_thickness) / 2]) ring(gasket_diameter + 0.1, hole_diameter, box_wall_thickness);

translate([0, 0, -0.1]) cylinder(d = cable_diameter, h = gasket_height + 0.2);
}