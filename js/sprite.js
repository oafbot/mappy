function Sprite(game, palette, dim, pix){
    var loc = [0, 0];

    this.frames = [];
    this.game = game;
    this.box = game.screen.group();
    this.sprite = this.box;
    this.dim = dim;
    this.pix = pix;
    this.palette = palette;
    this.frame = 0;
    this.direction = undefined;
    this.directions = {};
    this.animated = true;
    // this.init = function(){}

    this.draw = function(bitmap, offset, cols, flip){
        var q, ql, i, l, row, mod,

        cols = cols===undefined ? 2 : cols,
        flip = flip===undefined ? false : true,
        frame = game.screen.group();
        offset = offset === undefined ? {x : 0, y : 0} : offset;

        if(flip)
            bitmap = this.flipbits(bitmap);

        for(q=0, ql=bitmap.length; q<ql; q++){
            switch(q){
                case 0:
                    loc = [0, 0];
                    break;
                case 1:
                    loc[0] = dim*pix;
                    loc[1] = 0;
                    break;
                case 2:
                    loc[0] = 0;
                    loc[1] = dim * pix;
                    break;
                case 3:
                    loc[0] = dim * pix;
                    loc[1] = dim * pix;
                    break;
                default:
                    loc[0] = (dim * pix) * (q%cols);
                    loc[1] = (dim * pix) * (q-(q%4)-2);
            }

            for(i=0, l=bitmap[q].length; i<l; i++){
                row = parseInt(i/dim);
                mod = i%dim;

                if(palette[bitmap[q][i]]!==null)
                    frame.rect(pix, pix).move(offset.x + loc[0] + pix*mod, offset.y + loc[1] + row*pix).fill(palette[bitmap[q][i]]);
            }
        }
        frame.attr('class', 'bitmap')
        return frame;
    }

    this.animate = function(f, fn, timestamp){
        var frames = this.direction===undefined ? this.frames : this.directions[this.direction];
        // console.log(this.direction, frames.length);
        if(this.animated || frames.length>1){
            for(var i=0, l=frames.length; i<l; i++){
                if(i===f)
                    frames[i].opacity(1);
                else
                    frames[i].opacity(0);
            }
        }
        else{
            frames[0].opacity(1);
        }
        if(fn!==undefined)
            fn(f, timestamp);
    }

    this.group = function(sprite1, sprite2){
        var group = game.screen.group();
        group.add(sprite1);
        group.add(sprite2);
        return group;
    }

    this.add = function(){
        var group = this.box;
        for(var i=0, l=arguments.length; i<l; i++){
            group.add(arguments[i]);
            this.frames.push(arguments[i]);
        }
        return this;
    }

    this.define = function(direction, frames){
        this.directions[direction] = [];
        var group = this.box;
        for(var i=0, l=frames.length; i<l; i++){
            group.add(frames[i]);
            this.directions[direction].push(frames[i]);
        }
        return this;
    };

    this.move = function(x, y){
        this.box.dmove(x, y);
        return this;
    };

    this.position = function(x, y){
        if(x!==undefined && y!== undefined){
            this.box.move(x, y);
            return this;
        }
        return {x: this.box.cx(), y:this.box.cy()}
    };

    this.class = function(name){
        this.box.attr('class', name);
        return this;
    };

    this.classes = function(replace){
        var classes = this.box.classes();
        for(var i=0, l=classes.length; i<l; i++){
            this.box.removeClass(classes[i]);
        }
        for(var i=0, l=replace.length; i<l; i++){
            this.box.addClass(replace[i]);
        }
        return this;
    };

    this.factory = function(name, bitmaps, direction){
        var i, l, j, m, s
        frames = [];

        if(Object.keys(this.directions).length === 0 && this.directions.constructor === Object){
            s = new Sprite(this.game, this.palette, this.dim, this.pix);
        }
        else{
            s = this;
        }

        for(i=0, l=bitmaps.length; i<l; i++){
            var group = game.screen.group();
            for(j=0, m=bitmaps[i].length; j<m; j++){
                group.add(s.draw(bitmaps[i][j][0], bitmaps[i][j][1]));
            }
            frames.push(group)
        }

        if(direction!==undefined){
            s.define(direction, frames);
            for(i=0, l=s.directions[direction].length; i<l; i++)
                if(i!==0) s.directions[direction][i].opacity(0);
        }
        else{
            s.add(...frames);
            for(i=0, l=s.frames.length; i<l; i++)
                if(i!==0) s.frames[i].opacity(0);
        }

        if(game.cast.hasOwnProperty(name)){
            if(game.cast[name].indexOf(s)<0)
                game.cast[name].push(s);
        }
        else
            game.cast[name] = [s];

        return s;
    };

    this.bitmask = function(){
        if(this.mask!==undefined){
            return{
                x0: this.box.x()+this.mask.x()+pix,
                x1: this.box.x()+this.mask.x()+this.mask.bbox().width-pix,
                y0: this.box.y()+this.mask.y()+pix,
                y1: this.box.y()+this.mask.y()+this.mask.bbox().height-pix
            }
        }
        return {
            x0: this.box.x()+pix,
            x1: this.box.x()+this.box.bbox().width-pix,
            y0: this.box.y()+pix,
            y1: this.box.y()+this.box.bbox().height-pix
        }
    };

    this.collision = function(obj){
        var a = this.bitmask();
        var b = obj.bitmask();
        return !( a.x1 < b.x0 || a.x0 > b.x1 || a.y0 > b.y1 || a.y1 < b.y0 )
    };

    this.flipbits = function(bitmaps){
        var bitmaps1;
        var bitmaps2 = [];

        if(bitmaps.length==4){
            bitmaps1 = [bitmaps[1], bitmaps[0], bitmaps[3], bitmaps[2]];
        }
        else if(bitmaps.length==1){
            bitmaps1 = bitmaps;
        }
        else if(bitmaps.length==2){
            bitmaps1 = [bitmaps[1], bitmaps[0]];
        }
        else if(bitmaps.length==3){
            bitmaps1 = [bitmaps[2], bitmaps[1], bitmaps[0]];
        }

        for(var b=0, bl=bitmaps1.length; b<bl; b++){
            var bitmap = [];
            var row = [];
            for(var i=0, l=bitmaps1[b].length; i<=l; i++){
                if(i%this.dim===0){
                    if(row.length>0){
                        bitmap = bitmap.concat(row.reverse());
                    }
                    row = [bitmaps1[b][i]];
                }else{
                    row.push(bitmaps1[b][i]);
                }
            }
            bitmaps2.push(bitmap);
        }
        return bitmaps2;
    }

    this.flip = function(bitmap, offset, cols){
        return this.draw(bitmap, offset, cols, true);
    };

    this.show = function(){
        var frames = this.direction===undefined ? this.frames : this.directions[this.direction];
        if(this.animated){
            for(var i=0, l=frames.length; i<l; i++){
                if(game.frame!==i)
                    frames[i].opacity(0);
                else
                    frames[i].opacity(1);
            }
        }else{
            frames[0].opacity(1);
        }
        this.hidden = false;
        return this;
    };

    this.hide = function(){
        var frames = this.direction===undefined ? this.frames : this.directions[this.direction];
        for(var i=0, l=frames.length; i<l; i++){
            frames[i].opacity(0);
        }
        if(this.direction===undefined)
            this.hidden = true;
        return this;
    };

    this.hideAll = function(){
        var d, i, l, frames;
        for(d in this.directions){
            frames = this.directions[d];
            for(i=0, l=frames.length; i<l; i++){
                frames[i].opacity(0);
            }
        }
        this.hidden = true;
        return this;
    };

    this.visible = function(){
        if(this.hidden===undefined){
            var frames = this.direction===undefined ? this.frames : this.directions[this.direction];
            // console.log(this.direction, !frames.every(function(item){return item.opacity()===0;}));
            return !frames.every(function(item){return item.opacity()===0;})
        }
        return !this.hidden;
    };

    this.turn = function(direction){
        if(this.direction!=direction && this.direction!==undefined)
            this.hide();

        this.direction = direction;
        this.show();
        // this.animate(game.frame)
        return this;
    };

    this.width = function(){
        return this.sprite.bbox().width;
    };

    this.height = function(){
        return this.sprite.bbox().height;
    };

    this.center = function(){
        var position = this.position(),
            sprite = this.direction!==undefined ? this.directions[this.direction][0] : this.sprite;
            w = sprite.bbox().width,
            h = sprite.bbox().height;
        // console.log({ x: w/2, y:h/2 });
        return { x: w/2, y:h/2 };
    }
}