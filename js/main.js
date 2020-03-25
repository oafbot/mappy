SVG.on(document, 'DOMContentLoaded', function(){
const PIX = 2;
const DIM = 8;

const BYTE  = 8;
const WORD  = 16;
const LONG  = 32;

const DELAY = 350;
const WIDTH = 800;
const HEIGHT = 480;

var game,
    frames = {},
    screen = SVG('screen').attr('id', 'game'),
    b = bitmap,
    palette = [[null, "#000000", "#fcfcfc", "#f8f8f8", "#bcbcbc", "#7c7c7c", "#787878", "#a4e4fc", "#3cbcfc", "#0078f8", "#0000fc", "#b8b8f8", "#6888fc", "#0058f8", "#0000bc", "#d8b8f8", "#9878f8", "#6844fc", "#4428bc", "#f8d8f8", "#f8b8f8", "#f878f8", "#d800cc", "#940084", "#f8a4c0", "#f85898", "#e40058", "#a80020", "#f0d0b0", "#f87858", "#f83800", "#a81000", "#fce0a8", "#fca044", "#e45c10", "#881400", "#f8d878", "#f8b800", "#ac7c00", "#503000", "#d8f878", "#b8f818", "#00b800", "#007800", "#b8f8b8", "#58d854", "#00a800", "#006800", "#b8f8d8", "#58f898", "#00a844", "#005800", "#00fcfc", "#00e8d8", "#008888", "#004058"]];

    var init = function(){
        document.getElementById('screen').style.cursor = 'none';

        stage = screen.rect(WIDTH, HEIGHT).attr('id', 'backdrop');
        // stage.fill('#FCFCFC');

        game = new Game(screen);
        game.init({ bounds:{ top: 0, bottom: stage.y() + stage.bbox().height, left: 0, right: stage.x() + stage.bbox().width},
                    center:{ x:stage.bbox().width/2, y:stage.bbox().height/2 }
                 });
        set_player();
        // time = Math.floor(remaining/12.8)
        game.start(function(){ game.state = "start"; });
    };

    var set_player = function(){
        var frame  = [];

        player = new Sprite(game, palette[0], WORD, PIX);

        frame[0] = player.draw(player.flip([b.mappy[0]])).opacity(0);
        frame[1] = player.draw(player.flip([b.mappy[1]])).opacity(0);
        frame[2] = player.draw(player.flip([b.mappy[0]])).opacity(0);
        frame[3] = player.draw(player.flip([b.mappy[1]])).opacity(0);

        player.direction = "right";
        player = player.define('right', [frame[0], frame[1], frame[2], frame[3]]);

        game.layers.sprites.add(player.sprite);

        start = {
            x: stage.x() + stage.bbox().width/2  - player.sprite.bbox().width/2,
            y: stage.y() + stage.bbox().height/2 - player.sprite.bbox().height/2
        };

        player.sprite.move(start.x, start.y);
    };

    var update = function(){
        // var frame,
        // frame = game.frame;
        // var frame = player.directions[player.direction].length>4 ? game.frame2x :game.frame
        // player.animate(frame);
    };

    var tween = function(){
        player.animate(game.frame);
    };

    var reset = function(){
    };

    var run = function(){
        game.add(update, 'update');
        game.add(tween, 'tween');
        game.run();
    };

    init();
    run();
    window.game = game;
});