include <BOSL2/std.scad>
include <BOSL2/rounding.scad>

$fn = 32;

tray_radius = 80;
tray_height = 18;
tray_rounding = 18;
tray_flare = -6;
tray_inner_fillet = 2;
tray_wall_thickness = 2;

anchor_points = [
    LEFT,
    TOP,
    DOWN,
    RIGHT+DOWN,
    LEFT+DOWN,
    LEFT+UP,
    RIGHT+UP,
    "tip0"
];

cutout_radius = 4;
cutout_thickness = tray_wall_thickness * 4;
cutout_magic_offset = [0, -4.5, 4];

module tray_shell(radius) {
    reuleaux_polygon(n = 3, r = radius);
}
function tray_shell(radius) =
    reuleaux_polygon(n = 3, r = radius);
function round_tray_shell(radius) =
    round_corners(tray_shell(radius), radius = tray_rounding);
module swept_round_tray_shell(radius = tray_radius, flare = undef) {
    offset_sweep(round_tray_shell(radius), height = tray_height, bottom = flare, steps=$fn);
}

ripple_height = 0.8;
ripple_density = 12;
ripple_function = function (x,y) ripple_height * cos(ripple_density * norm([x,y]));

module ripple() {
translate([0, 0, ripple_height]) intersection() {
    heightfield(size=[tray_radius * 2, tray_radius * 2], bottom=-2, data=ripple_function, xrange=[-180:2:180], yrange=[-180:2:180]);
    translate([0, 0, -tray_height / 2]) linear_extrude(tray_height) tray_shell(tray_radius - (tray_wall_thickness * 2) + tray_wall_thickness / 2);
}
}

difference() {
    swept_round_tray_shell(flare = os_circle(r = tray_flare));
    translate([0, 0, tray_wall_thickness]) swept_round_tray_shell(radius = tray_radius - tray_wall_thickness * 2, flare = os_smooth(joint = tray_inner_fillet));
    /*for(i = [0 : len(anchor_points) - 1]) {
        anchor_point = anchor_points[i];

        hide("hide") tag("hide") reuleaux_polygon(n = 3, r = tray_radius - tray_wall_thickness * 2) tag("keep") attach(anchor_point) translate(cutout_magic_offset) rotate([-90, 0, 0]) cylinder(r = cutout_radius, h = cutout_thickness);
    }*/
}

translate([0, 0, -6]) ripple();
