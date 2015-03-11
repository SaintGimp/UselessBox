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
		CAG.circle({center: [50, 0], radius: 2}),
	];

	return chain_hull(armNodes).subtract(head_cutout);
}

function server_cutout() {
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
	var width = 40;
	var height = 70;

	var bracket = CAG.rectangle({center: [width/2, height/2], radius: [width/2, height/2]});
	var cutout = server_cutout().translate([10, 28]);

	return bracket.subtract(cutout);
}

function switch_servo_bracket() {
	var width = 64;
	var height = 60;

	var bracket = CAG.rectangle({center: [width/2, height/2], radius: [width/2, height/2]});
	var cutout = server_cutout()
		.rotateZ(90)
		.translate([width / 2 - 3, 10]);

	return bracket.subtract(cutout);
}

function lid_bracket_brace() {
	var width = 36;
	var height = 70;
	return CAG.rectangle({center: [width/2, height/2], radius: [width / 2, height / 2]});
}

function servo_bracket_brace() {
	var width = 36;
	var height = 60;
	return CAG.rectangle({center: [width/2, height/2], radius: [width / 2, height / 2]});
}

function main() {
	return switch_arm().translate([7, -6])
		.union(lid_arm().translate([50, 20]))
		.union(lid_servo_bracket().translate([66, 30]))
		.union(switch_servo_bracket().translate([0, 30]))
		.union(lid_bracket_brace().translate([-40, 30]))
		.union(servo_bracket_brace().translate([-80, 30]));

}