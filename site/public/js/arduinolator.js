Object.keys = Object.keys || function(o) {
    var result = [];
    for(var name in o) {
        if (o.hasOwnProperty(name))
          result.push(name);
    }
    return result;
};

Object.prototype.keys = function () {
    return Object.keys(this);
}


var HIGH = 1;
var LOW  = 0;
var in_count = 5;
var out_count = 5;
var events = {};
var mapping = {};
var alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
$(function () {
    $('#config_pin').click(function () {
        $("#step_one").show();
        
        return false;
    });
    
    function update_script() {
        var result = alphabet.substr(0, in_count);
        
        result += "\n---\n";
        result += alphabet.substr(in_count, out_count);
        result += "\n---\n";
        
        for (var a in mapping) {
            var foo = mapping[a];
            result += "Pin " +  
                      foo['input'] + ':' + foo['output'] + ':' + 
                      foo['cor'] + ':' + foo['tim'] + ':' + 
                      eval(foo['rat']) + "\n";
        }
        
        // $("#stuff").text(result);
    }
    
    function io_update() {
        $("#input_list").text("a-" + alphabet.charAt(in_count));
        $("#output_list").text(alphabet.charAt(in_count + 1) + "-" + alphabet.charAt(in_count+out_count));
        mapping = {};
        update_script();
        $("#mapping_listing").html("");
    }
    
    io_update();
    
    $("#event_type").change(function () {
        $("#event_type_specification").html("");
        $("#event_type_" + ($("#event_type").val()).toString().toLowerCase())
            .tmpl().appendTo("#event_type_specification");
    }).change();
    
    $('#update_io_count').click(function () {
        in_count = parseInt($("#input_count").val());
        out_count = parseInt($("#output_count").val());
        
        io_update();
        
        return false;
    });
    
    function debug(obj) {
        var vals = [];
        for (var key in obj) {
            if (obj[key])
                vals.push(key + " : [" + typeof(obj[key]) + "] " + obj[key]);
        }
        return $("<pre><code>").html(vals.join("\n<br />")).html();
    }
    
    $("#events_list").change(function () {
        
    });
    
    $("#remove").click(function () {
        alert("clicked me");
        return false;
    });
    
    $("#mapping_listing").change(function () {
        $("#info").html("");
        $("#mapping_listing option:selected").each(function () {
            $("#mapping_template").tmpl(mapping[$(this).val()]).appendTo("#info");
        });
    });

    $("#add_event").click(function () {
        mapping[$("#id").val()] = {
            'id' : $("#id").val(),
            'type' : $("#e_type").val(),
            'ivo' : $("#ivo").val(),
            'val' : $("#val").val()
        };
        
        $("#"+ $("#ivo").val() +"put_listing").append("<option value=\"" + $("#id").val() + "\">" 
            + $("#id").val() + "</option>");
        $("").append("<option value=\"" + $("#id").val() + "\">"
            + $("#id").val() + "</option>");
        return false;
    });
    
    $("#add_mapping").click(function () {
        // Validate Parameters
        //   If the map is empty
        if (mapping.length == 0) {
            
        }
        var id = mapping.keys().join('');
        events[id] = mapping;
        $("#events_list").append("<option value=\"" + id + "\">" + id + "</option>");
        mapping = {};
        $('#input_listing, #output_listing').html('');
        return false;
    });
    
    update_script();
    
    var last = (new Date()).getTime();
    
    /* Create Grid */
    var pix_to_time = 4;
    var sf = 20; /* scale factor */
    /* Framerate controls */
    var frames = 5; /* Per Second */
    var need_to_redraw = false;
    var canvas = $("#canvas");
    var canvas_width = canvas.width();
    var canvas_height = canvas.height();
    var ctx = canvas[0].getContext("2d");
    
    $("#canvas").data({"evts" : [], 
                       "registered_objs" : []});
    
    function draw_line(x, y, dx, dy, color) {
        var reg = $("#canvas").data("registered_objs");
        
        reg.push({ 'shape' : function (ctx) {
            ctx.fillStyle = color;
            ctx.strokeStyle = color;
            ctx.lineWidth = 1;
            ctx.beginPath();
            ctx.moveTo(x, y);
            ctx.lineTo(dx, dy);
            ctx.stroke();
        }});
        
        need_to_redraw = true;
    }
    
    function persist(f) {
        var reg = $("#canvas").data("registered_objs");
        reg.push({'shape' : function () {
            f(ctx);
        }});
    }

    function redraw() {
        if (need_to_redraw) {
            canvas.width(canvas.width());
            ctx.clearRect(0, 0, canvas_width, canvas_height); // clear canvas
            for (var i = (10 * pix_to_time); i < canvas_width; i += (10 * pix_to_time)) {
                ctx.fillStyle = 'rgb(0, 0, 0)';
                ctx.fillText(i / pix_to_time, i - 6, 10);
                draw_line(i, 0, i, canvas_height, 'rgb(60, 60, 60)');
            }
            var reg_objs = $("#canvas").data("registered_objs");
            for (var i in reg_objs) {
                reg_objs[i].shape(ctx);
            }
        
            need_to_redraw = false;
        }
    
        var new_time = (new Date()).getTime();
        // $("#frames").text(new_time 
        // + " and " + last + " frames " + (1 / ((new_time - last)/1000)));
        // setTimeout(redraw, (1 / frames));
        last = new_time;
    }

    function mk_btn(shape, is_clicked, cb, up) {
        var reg = $("#canvas").data("registered_objs");
        reg.push({'shape': function () {
            shape(ctx);
        }});
        var evts = $("#canvas").data('evts');
        evts.push({'check' : is_clicked, 'cb' : cb, 'up' : up});
        need_to_redraw = true;
    }

    function square_wave(height, period, interval, start) {
        var start = 0;
    
        need_to_redraw = true;
    }

    function validate() {
    
    }

    $("select[name=default_configuration]").change(function() {
        $(this).val();
    });
    
    $("#d_pin_count").keyup(function (evt) {
        validate();
    });
    $("#a_pin_count").keyup(function (evt) {
        validate();
    });
    
    canvas.mousemove(function (evt) {
        $("#mouse_pos").text(evt.offsetX + " and " + evt.offsetY);
    });
    
    canvas.mousedown(function (evt) {
        // drag event
        var evts = $(this).data('evts');
        $.each($.grep(evts, function (n, i) {
            return !n.up && n.check(evt.offsetX, evt.offsetY);
        }), function () {
            this.cb();
        });
        
        $("#mouse_down").html("Down pressed ").append(debug(evt));
    });
    
    canvas.mouseup(function (evt) {
        var evts = $(this).data('evts');
        $.each($.grep(evts, function (n, i) {
            return n.up && n.check(evt.offsetX, evt.offsetY);
        }), function () {
            this.cb();
        });
        
        $("#mouse_down").html("Up event fired ").append(debug(evt));
    });
    
    function draw_grid() {
        ctx.save();
        ctx.lineWidth = 1;
        ctx.strokeStyle = "rgba(55, 55, 55, 0.5)";
        for (var i=1; i <= sf; i++) {
            ctx.moveTo(i * sf, 0);
            ctx.lineTo(i * sf, canvas_height);
        }
        ctx.stroke();
        ctx.restore();
    }
    
    draw_grid();
    
    var items = 0;
    
    function draw_serial(name, row) {
        items++;
        var y = items*50;
        console.log("y is : " + y);
        var x = 0;
        ctx.save();
        ctx.font = "22px 'Linux Libertine', Palatino, 'Palatino Linotype', 'Book Antiqua', Georgia, 'Times New Roman', serif";
        ctx.fillText("Serial Signal: " + name, 5, y - 6);
        ctx.strokeStyle = 'rgb(255, 0, 0)';
        ctx.lineWidth = 2;
        ctx.font = "12px 'Linux Libertine', Palatino, 'Palatino Linotype', 'Book Antiqua', Georgia, 'Times New Roman', serif";
        ctx.beginPath();
        ctx.moveTo(x, y);
        ctx.rotate(Math.PI/2);
        for (var i in row) {
            ctx.moveTo(y, -1*row[i][0]*sf);
            ctx.lineTo(y + 50, -1*row[i][0]*sf);
            ctx.stroke();
            ctx.fillText(row[i][1], y, -1*row[i][0]*sf - 3);
        }
        console.log("Done?");
        ctx.closePath();
        ctx.restore();
    }
    function draw_pin(name, row) {
        items++;
        var y = items*50;
        console.log("y is : " + y);
        var x = 0;
        var state = 0;
        ctx.save();
        ctx.font = "22px 'Linux Libertine', Palatino, 'Palatino Linotype', 'Book Antiqua', Georgia, 'Times New Roman', serif";
        ctx.fillText("Digital Signal: " + name, 5, y - 6);
        
        ctx.strokeStyle = 'rgb(255, 0, 0)';
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.moveTo(x, y);
        for (var i in row) {
            if (state == row[i][0]) {
            }
            else {
                if (state == 0) {
                    /* move up */
                    y += 15;
                    ctx.lineTo(x, y);
                    state = 1;
                }
                else {
                    /* Move down */
                    y -= 15;
                    ctx.lineTo(x, y);
                    state = 0;
                }
            }
            x += row[i][1] * sf;
            ctx.lineTo(x, y);
            console.log(x + " and " + y + " or " + row[i]);
        }
        ctx.lineWidth = 1;
        ctx.stroke();
        ctx.restore();
    }
    
    redraw();
    $.ajax({
        url: 'http://localhost:3000/js/sample.js',
        dataType: 'json',
        success: function (data) {
            for (var y in data['pins']) {
                draw_pin(y, data['pins'][y]);
            }
            for (var y in data['serial']) {
                draw_serial(y, data['serial'][y]);
            }
        }
    });
});
