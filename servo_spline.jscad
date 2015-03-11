// Based on https://github.com/hugokernel/OpenSCAD_ServoArms

ServoSpline = (function() {
    var module = {};

    /**
     *  Clear between arm head and servo head
     */
    module.PLA_SERVO_HEAD_CLEAR = 0.3;
    module.ABS_SERVO_HEAD_CLEAR = 0.2;
    module.LASER_SERVO_HEAD_CLEAR = -0.15;

    /**
     *  Head / Tooth parameters
     *  Futaba 3F Standard Spline
     *  http://www.servocity.com/html/futaba_servo_splines.html
     *
    */
    module.FUTABA_3F_SPLINE = {
        headExternalDiameter: 5.92,
        headHeight: 4,
        headThickness: 1.1,
        headScrewDiameter: 2.5,

        toothCount: 25,
        toothHeight: 0.3,
        toothLength: 0.7,
        toothWidth: 0.1
    };

module.TOWER_3F_SPLINE = {
        headExternalDiameter: 5.68,
        headHeight: 4,
        headThickness: 1.1,
        headScrewDiameter: 2.5,

        toothCount: 25,
        toothHeight: 0.3,
        toothLength: 0.7,
        toothWidth: 0.1
    };

    /**
     *  Tooth
     *
     *    |<-w->|
     *    |_____|___
     *    /     \  ^h
     *  _/       \_v
     *   |<--l-->|
     *
     *  - tooth length (l)
     *  - tooth width (w)
     *  - tooth height (h)
     *  - height
     *
     */
    function servo_head_tooth(length, width, height) {
        return CAG.fromPoints([[-length / 2, 0], [-width / 2, height], [width / 2, height], [length / 2,0]]);
    }

    module.head_2d = function(params, clear) {
        var head = CAG.circle({center: [0, 0], radius: params.headExternalDiameter / 2 - params.toothHeight + 0.03 + clear});

        for (var i = 0; i < params.toothCount; i++) {
            var tooth = servo_head_tooth(params.toothLength, params.toothWidth, params.toothHeight);
            tooth = tooth.translate([0, params.headExternalDiameter / 2 - params.toothHeight + clear, 0]);
            tooth = tooth.rotateZ(i * (360 / params.toothCount));
            head = head.union(tooth);
        }

        return head;
    }

    module.head_3d = function(params, clear) {
        return head_2d(params, clear).extrude([0, 0, params.headHeight]);
    }

    return module;
})();
