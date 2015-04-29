include("servo_spline.jscad");

function switch_arm() {
	var head_cutout = ServoSpline.head_2d(ServoSpline.TOWER_3F_SPLINE, ServoSpline.LASER_SERVO_HEAD_CLEAR).scale([1,1,5]);

	var segmentLength = 35;
	var armNodes = [
		CAG.circle({center: [0, 0], radius: 7}),
		CAG.circle({center: [6, 0], radius: 4}),
		CAG.circle({center: [30, 0], radius: 4}),
		CAG.circle({center: [30, 48], radius: 4}),
		CAG.circle({center: [10, 48], radius: 4})
	];

	return chain_hull(armNodes).subtract(head_cutout);

}

function lid_arm() {
	var head_cutout = ServoSpline.head_2d(ServoSpline.TOWER_3F_SPLINE, ServoSpline.LASER_SERVO_HEAD_CLEAR).scale([1,1,5]);

	var armNodes = [
		CAG.circle({center: [0, 0], radius: 7}),
		CAG.circle({center: [37, 0], radius: 3}),
	];

	return chain_hull(armNodes).subtract(head_cutout);
}

function servo_cutout() {
	body = CAG.rectangle({center: [0, 0], radius: [10, 20]});
	holes = [
		CAG.circle({center: [5, 24.4], radius: 1.5}),
		CAG.circle({center: [-5, 24.4], radius: 1.5}),
		CAG.circle({center: [5, -24.4], radius: 1.5}),
		CAG.circle({center: [-5, -24.4], radius: 1.5})
	];
	
	return body.union(holes);
}

function lid_servo_bracket() {
	var width = 35;
	var height = 70;

	var bracket = CAG.rectangle({center: [width/2, height/2], radius: [width/2, height/2]});
	var cutout = servo_cutout().translate([10, 28]);
	var limitSwitchCutout = CAG.rectangle({center: [width/2, height/2], radius: [7, height/4]})
		.translate([width/2, 0]);
	return bracket.subtract([cutout, limitSwitchCutout]);
}

function switch_servo_bracket() {
	var width = 87;
	var height = 60;

	var bracket = CAG.rectangle({center: [width/2, height/2], radius: [width/2, height/2]});
	var cutout = servo_cutout()
		.rotateZ(90)
		.translate([29, 10]);

	return bracket.subtract(cutout);
}

function lid_bracket_brace() {
	var width = 32;
	var height = 70;
	
	var brace = CAG.rectangle({center: [width/2, height/2], radius: [width / 2, height / 2]});
	var hole1 = CAG.circle({center: [26, 43], radius: 1.25});
	var hole2 = CAG.circle({center: [16.4, 43], radius: 1.25});
	return brace.subtract([hole1, hole2]);
}

function servo_bracket_brace() {
	var width = 37.8;
	var height = 60;
	return CAG.rectangle({center: [width/2, height/2], radius: [width / 2, height / 2]});
}

function main() {
	return switch_arm().translate([-17, -6])
		.union(lid_arm().translate([47, 20]))
		.union(lid_servo_bracket().translate([66, 30]))
		.union(switch_servo_bracket().translate([-23, 30]))
		.union(lid_bracket_brace().translate([-63, 30]))
		.union(servo_bracket_brace().translate([-103, 30]));

}