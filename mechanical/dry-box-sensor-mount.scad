include <BOSL2/std.scad>
include <BOSL2/rounding.scad>

$fn = 32;

d1_mini_dims = [25.6, 33.3, 1.6];

case_fillet = 2;
case_padding = 4;
case_rounding = 4;
case_height = 2.6;
case_inner_dims = [d1_mini_dims[0] + (case_padding * 2), d1_mini_dims[1] + (case_padding * 2)];

hole_diameter = 3 + 0.3;
hole_offset = 20.65;
hole_offsets = [[(d1_mini_dims[0] - hole_offset) / 2, 29.75], [(d1_mini_dims[0] + hole_offset) / 2, 29.75]];
insert_height = 3.5 * 1.5;

standoff_padding = 0.8;
standoff_height = insert_height + 1;
standoff_diameter = hole_diameter + (standoff_padding * 2);

prop_dims = [14, 6, standoff_height];
prop_offset = [0, -(d1_mini_dims[1] - 10) / 2, 0];

module case() {
union() {
translate([0, 0, case_height / 2]) offset_sweep(rect(case_inner_dims, rounding = case_rounding), top=os_smooth(joint = case_fillet / 2), height = case_height, steps=$fn);
translate([prop_offset[0], prop_offset[1], case_height + prop_dims[2] / 2]) offset_sweep(rect([prop_dims[0], prop_dims[1]], rounding = 1), top=os_chamfer(width = 1), height = prop_dims[2], steps = $fn);
}
}

module standoff() {
union() {
difference() {
    cylinder(d = standoff_diameter, h = standoff_height);
    translate([0, 0, standoff_height - insert_height]) cylinder(d = hole_diameter, h = insert_height);
}
rotate_extrude() translate([standoff_diameter / 2, 0, 0]) mask2d_teardrop(d = standoff_diameter);
}
}

/*%color("blue") render() translate([d1_mini_dims[0] / 2, -d1_mini_dims[1] / 2, 10]) rotate([180, 0, 180]) difference() {
    cube(d1_mini_dims);
    for(i = [0 : len(hole_offsets) - 1]) {
        hole_offset = hole_offsets[i];
        translate([hole_offset[0], hole_offset[1], 0]) cylinder(d = hole_diameter, h = d1_mini_dims[2]); 
    }
}*/

union() {
case();

translate([-d1_mini_dims[0] / 2, -d1_mini_dims[1] / 2, case_height]) {
    for(i = [0 : len(hole_offsets) - 1]) {
        hole_offset = hole_offsets[i];
        translate([hole_offset[0], hole_offset[1], 0]) standoff();
    }
}
}