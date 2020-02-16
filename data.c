struct Data data = {
    .palette = {"NULL", "#000000", "#fcfcfc", "#f8f8f8", "#bcbcbc", "#7c7c7c", "#787878", "#a4e4fc", "#3cbcfc", "#0078f8", "#0000fc", "#b8b8f8", "#6888fc", "#0058f8", "#0000bc", "#d8b8f8", "#9878f8", "#6844fc", "#4428bc", "#f8d8f8", "#f8b8f8", "#f878f8", "#d800cc", "#940084", "#f8a4c0", "#f85898", "#e40058", "#a80020", "#f0d0b0", "#f87858", "#f83800", "#a81000", "#fce0a8", "#fca044", "#e45c10", "#881400", "#f8d878", "#f8b800", "#ac7c00", "#503000", "#d8f878", "#b8f818", "#00b800", "#007800", "#b8f8b8", "#58d854", "#00a800", "#006800", "#b8f8d8", "#58f898", "#00a844", "#005800", "#00fcfc", "#00e8d8", "#008888", "#004058"},

    .sprites = {{
        {0,0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,4,24,24,4,0,9,9,0,4,24,24,4,0,0,0,4,24,24,24,4,9,9,9,9,24,24,24,24,4,0,0,4,24,24,24,13,36,36,13,9,24,24,24,24,4,0,0,4,24,9,9,9,9,9,9,9,24,24,24,24,4,0,0,0,4,24,2,2,4,2,2,4,4,24,24,24,4,0,0,0,0,4,1,2,4,1,2,4,4,24,24,4,0,37,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,37,0,0,33,4,4,4,4,4,4,4,4,9,4,4,0,0,37,4,9,9,2,2,9,9,9,9,9,9,4,4,0,0,0,4,9,9,9,9,9,9,9,9,0,0,0,0,0,0,38,38,9,36,9,9,36,9,9,36,9,38,0,0,0,0,38,38,38,9,9,9,9,9,9,9,9,38,38,0,0,0,38,38,38,9,9,9,9,9,9,9,38,38,38,0,0,0,0,38,38,0,0,0,0,0,0,9,38,38,38,0,0,0,0,0,0,0,0,0,0,0,0,38,38,38,0,0},
        {0,0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,4,24,24,4,0,9,9,0,4,24,24,4,0,0,0,4,24,24,24,4,9,9,9,9,24,24,24,24,4,0,0,4,24,24,24,13,36,36,13,9,24,24,24,24,4,0,0,4,24,9,9,9,9,9,9,9,24,24,24,24,4,0,0,0,4,24,2,2,4,2,2,4,4,24,24,24,4,0,37,0,0,4,1,2,4,1,2,4,4,24,24,4,0,0,37,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,37,4,0,33,4,4,4,4,4,4,4,4,0,0,0,0,4,4,9,9,9,2,2,9,9,9,9,9,0,0,0,0,4,4,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,36,9,9,36,9,9,36,0,4,4,0,0,0,0,0,0,9,9,9,9,9,9,9,0,4,4,0,0,0,0,0,0,0,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,38,38,38,38,0,0,0,0,0,0,0,0,0,0,0,38,38,38,38,38,0,0,0,0,0},
        // drop
        {0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,4,24,24,4,0,9,9,0,4,24,24,4,0,0,0,4,24,24,24,4,9,9,9,9,4,24,24,24,4,0,0,4,24,24,24,24,13,36,36,13,24,24,24,24,4,0,0,4,24,24,24,9,9,9,9,9,9,24,24,24,4,0,0,0,4,24,4,2,2,4,2,2,4,4,24,4,0,0,0,0,0,4,4,2,1,4,1,2,4,4,4,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,4,4,9,4,4,33,33,4,4,4,4,9,4,4,0,0,4,4,9,9,9,2,2,9,9,9,9,9,4,4,0,0,0,0,0,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,36,9,9,36,9,9,36,9,0,0,0,0,0,0,0,38,38,9,9,9,9,9,9,38,38,0,0,0,0,0,0,38,38,38,9,0,0,9,38,38,38,0,0,0,0,0,0,38,38,38,0,0,0,0,38,38,38,0,0,0,0,0,0,0,38,38,0,0,0,0,38,38,0,0,0,0},
        {0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,4,24,24,4,0,9,9,0,4,24,24,4,0,0,0,4,24,24,24,4,9,9,9,9,4,24,24,24,4,0,0,4,24,24,24,24,13,36,36,13,24,24,24,24,4,0,0,4,24,24,24,9,9,9,9,9,9,24,24,24,4,0,0,0,4,24,4,2,2,4,2,2,4,4,24,4,0,0,0,0,0,4,4,2,1,4,1,2,4,4,4,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,4,4,33,33,4,4,4,4,0,0,0,0,0,0,0,0,9,9,2,2,9,9,9,9,0,0,0,0,0,0,4,9,9,9,9,9,9,9,9,9,9,4,0,0,0,4,4,9,36,9,9,36,9,9,36,9,9,4,4,0,0,4,0,38,38,9,9,9,9,9,9,38,38,0,4,0,0,0,0,38,38,38,9,0,0,9,38,38,38,0,0,0,0,0,0,38,38,38,0,0,0,0,38,38,38,0,0,0,0,0,0,0,38,38,0,0,0,0,38,38,0,0,0,0},
        // jump
        {0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,4,4,4,4,0,9,9,0,4,4,4,4,0,0,0,4,4,4,4,4,9,9,9,9,4,4,4,4,4,0,0,4,4,4,4,4,9,9,9,9,4,4,4,4,4,0,0,4,4,4,4,9,9,9,9,9,9,4,4,4,4,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,0,0,0,0,4,4,9,9,4,4,4,4,4,4,9,9,4,4,0,0,4,4,9,9,9,9,9,9,9,9,9,9,4,4,0,0,0,0,0,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,38,9,36,9,9,36,9,9,36,38,0,0,0,0,0,38,38,9,9,9,9,9,9,9,9,38,38,0,0,0,0,38,38,9,9,9,0,0,9,9,9,38,38,0,0,0,0,0,38,38,0,0,0,0,0,0,38,38,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,4,24,24,4,0,9,9,0,4,24,24,4,0,0,0,4,24,24,24,4,9,9,9,9,24,24,24,24,4,0,0,4,24,24,24,13,36,36,13,9,24,24,24,24,4,37,0,4,24,9,9,9,9,9,9,9,24,24,24,24,4,0,37,0,4,24,2,2,4,2,2,4,4,24,24,24,4,0,0,37,0,4,1,2,4,1,2,4,4,24,24,4,0,0,0,4,4,0,4,4,4,4,4,4,4,4,4,0,4,0,0,4,4,33,4,4,4,4,4,4,4,4,9,4,4,0,0,0,9,9,9,2,2,9,9,9,9,9,9,4,0,0,0,38,38,9,9,9,9,9,9,9,9,0,0,0,0,0,0,38,38,9,36,9,9,36,9,9,36,9,0,0,0,0,0,38,38,38,9,9,9,9,9,9,9,9,38,38,0,0,0,0,38,38,9,9,0,0,0,0,9,38,38,38,38,0,0,0,38,38,0,0,0,0,0,0,0,0,38,38,38,0,0,0,0,0,0,0,0,0,0,0,0,0,0,38,38},
        {0,0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0,0,4,24,24,4,0,9,9,0,4,24,24,4,0,0,0,4,24,24,24,4,9,9,9,9,24,24,24,24,4,0,0,4,24,24,24,13,36,36,13,9,24,24,24,24,4,0,0,4,24,9,9,9,9,9,9,9,24,24,24,24,4,37,0,0,4,24,2,2,4,2,2,4,4,24,24,24,4,0,37,0,0,4,1,2,4,1,2,4,4,24,24,4,0,0,0,37,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0,4,33,4,4,4,4,4,4,4,4,0,0,0,0,0,4,4,9,9,2,2,9,9,9,9,9,9,0,0,0,0,38,38,9,9,9,9,9,9,9,9,9,9,4,4,0,0,38,38,38,36,9,9,36,9,9,36,9,0,4,4,0,0,38,38,38,9,9,9,9,9,9,9,38,0,0,0,0,0,0,38,38,0,0,9,9,9,9,38,38,38,0,0,0,0,0,0,0,0,0,0,0,0,38,38,38,38,0,0,0,0,0,0,0,0,0,0,0,38,38,38,38,0,0,0},
        // hit
        {0,0,0,37,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,37,0,0,4,24,24,24,4,4,0,0,0,0,0,0,0,37,0,0,4,9,9,24,4,4,0,0,0,0,0,4,4,37,0,0,4,24,9,9,9,9,9,0,0,0,0,4,4,4,0,0,2,2,4,9,9,36,9,9,0,0,0,9,9,9,33,4,2,1,4,4,9,9,9,4,4,0,0,0,9,9,9,4,4,4,1,2,4,9,24,24,24,4,38,0,9,9,9,2,4,4,2,2,4,24,24,24,24,4,38,38,38,9,9,9,2,4,4,4,4,24,24,24,24,4,38,38,38,9,9,9,9,9,4,4,4,24,24,4,4,0,0,38,38,9,36,9,9,9,9,4,4,4,4,0,0,0,0,0,0,0,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,9,9,9,36,9,9,9,4,4,0,0,0,0,0,38,38,38,9,9,0,0,9,9,4,4,0,0,0,0,0,0,38,38,38,38,0,0,0,0,4,0,0,0,0,0,0,0,0,38,38,38,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,37,0,0,0,0,0,0,0,0,0,38,38,38,4,0,37,0,0,4,4,4,0,0,0,0,38,38,38,38,4,4,0,0,4,24,24,24,4,0,0,0,38,38,38,9,9,33,0,4,24,9,24,24,24,4,0,0,0,9,36,9,9,4,4,1,2,9,24,24,24,4,0,0,0,9,9,9,2,4,4,2,2,9,13,4,4,0,0,0,9,9,9,9,2,4,4,4,4,9,36,9,0,0,0,0,9,9,36,9,9,4,4,1,2,9,36,9,9,0,38,0,9,9,9,9,9,4,4,2,2,9,13,9,9,0,38,38,9,9,9,9,9,4,4,4,4,9,9,9,0,0,38,38,38,9,36,9,9,4,4,4,4,24,24,24,4,0,38,38,38,38,9,9,9,4,4,24,24,24,24,24,24,4,0,38,38,0,0,9,9,0,4,24,24,24,24,24,24,4,0,0,0,0,4,4,0,0,0,4,24,24,24,24,4,0,0,0,0,0,4,4,0,0,0,0,4,4,4,4,0,0},
        {0,0,0,0,0,0,38,38,38,0,0,0,0,0,0,0,0,0,0,0,0,38,38,38,0,0,9,4,4,0,0,0,0,0,38,0,0,38,38,38,9,9,9,4,4,0,0,0,0,38,38,0,0,9,9,9,9,9,9,4,37,37,37,37,38,38,38,9,9,36,9,9,9,9,33,0,0,0,0,0,38,38,9,9,9,9,9,9,2,4,4,0,0,0,0,0,38,38,9,9,9,9,9,2,4,4,2,2,4,4,4,0,0,0,0,36,9,9,9,4,4,4,1,2,24,9,24,4,0,0,0,9,9,9,4,4,2,1,4,4,9,9,24,4,0,0,9,9,9,4,4,4,2,2,4,9,9,24,24,4,0,0,9,9,0,4,4,4,4,4,9,9,9,4,4,4,0,4,4,4,0,4,24,24,24,9,9,36,9,4,4,0,0,0,4,4,0,4,24,24,24,24,9,9,9,0,0,0,0,0,0,0,0,0,4,24,24,24,4,9,0,0,0,0,0,0,0,0,0,0,4,24,24,24,4,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,0},
        {0,0,0,38,38,38,38,0,0,0,0,0,0,0,0,0,0,0,38,38,38,38,0,0,0,0,0,0,0,0,0,0,0,0,38,38,38,9,9,9,9,0,0,38,38,0,0,0,0,0,0,38,9,9,9,9,9,9,9,38,38,38,0,0,4,4,0,9,36,9,9,36,9,9,36,38,38,38,0,0,4,4,9,9,9,9,9,9,9,9,9,9,38,38,0,0,0,0,9,9,9,9,9,9,2,2,9,9,4,4,0,0,0,0,0,4,4,4,4,4,4,4,4,33,4,0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,37,0,0,0,4,24,24,4,4,2,1,4,2,1,4,0,0,37,0,4,24,24,24,4,4,2,2,4,2,2,24,4,0,0,37,4,24,24,24,24,9,9,9,9,9,9,9,24,4,0,0,4,24,24,24,24,9,13,36,36,13,24,24,24,4,0,0,4,24,24,24,24,9,9,9,9,4,24,24,24,4,0,0,0,4,24,24,4,0,9,9,0,4,24,24,4,0,0,0,0,0,4,4,0,0,0,0,0,0,4,4,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,38,38,38,0,0,0,0,0,0,0,0,4,0,0,0,0,38,38,38,38,0,0,0,0,0,0,4,4,9,9,0,0,9,9,38,38,38,0,0,0,0,0,4,4,9,9,9,36,9,9,9,0,0,0,0,0,0,0,0,0,0,9,9,9,9,9,9,0,0,0,0,0,0,0,4,4,4,4,9,9,9,9,36,9,38,38,0,0,4,4,24,24,4,4,4,9,9,9,9,9,38,38,38,4,24,24,24,24,4,4,4,4,2,9,9,9,38,38,38,4,24,24,24,24,4,2,2,4,4,2,9,9,9,0,38,4,24,24,24,9,4,2,1,4,4,4,9,9,9,0,0,0,4,4,9,9,9,4,4,1,2,4,33,9,9,9,0,0,0,9,9,36,9,9,4,2,2,0,0,4,4,4,0,0,0,0,9,9,9,9,9,24,4,0,0,37,4,4,0,0,0,0,0,4,4,24,9,9,4,0,0,37,0,0,0,0,0,0,0,4,4,24,24,24,4,0,0,37,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,37,0,0,0},
        {0,0,4,4,4,4,0,0,0,0,4,4,0,0,0,0,0,4,24,24,24,24,4,0,0,0,4,4,0,0,0,0,4,24,24,24,24,24,24,4,0,9,9,0,0,38,38,0,4,24,24,24,24,24,24,4,4,9,9,9,38,38,38,38,0,4,24,24,24,4,4,4,4,9,9,36,9,38,38,38,0,0,9,9,9,4,4,4,4,9,9,9,9,9,38,38,0,9,9,13,9,2,2,4,4,9,9,9,9,9,0,38,0,9,9,36,9,2,1,4,4,9,9,36,9,9,0,0,0,0,9,36,9,4,4,4,4,2,9,9,9,9,0,0,0,4,4,13,9,2,2,4,4,2,9,9,9,0,0,0,4,24,24,24,9,2,1,4,4,9,9,36,9,0,0,0,4,24,24,24,9,24,4,0,33,9,9,38,38,38,0,0,0,4,24,24,24,4,0,0,4,4,38,38,38,38,0,0,0,0,4,4,4,0,0,37,0,4,38,38,38,0,0,0,0,0,0,0,0,0,37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,37,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,4,24,24,24,4,0,0,0,0,0,0,0,0,0,0,9,4,24,24,24,4,0,0,0,0,0,0,0,0,0,9,9,9,24,24,24,24,4,0,4,4,0,0,0,4,4,9,36,9,9,24,24,24,4,0,4,4,4,0,4,4,4,9,9,9,4,4,4,4,4,0,9,9,0,0,4,24,24,9,9,4,2,2,4,4,4,9,9,9,0,0,4,24,9,9,4,4,1,2,4,4,9,9,9,0,0,0,4,24,9,24,2,1,4,4,4,9,9,9,36,0,0,0,0,4,4,4,2,2,4,4,2,9,9,9,9,9,38,38,0,0,0,0,0,4,4,2,9,9,9,9,9,9,38,38,0,0,0,0,0,33,9,9,9,9,36,9,9,38,38,38,37,37,37,37,4,9,9,9,9,9,9,0,0,38,38,0,0,0,0,4,4,9,9,9,38,38,38,0,0,38,0,0,0,0,0,4,4,9,0,0,38,38,38,0,0,0,0,0,0,0,0,0,0,0,0,38,38,38,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,37,0,0,0,0,0,0,0,0,0,0,0,4,4,37,37,0,0,0,0,0,0,0,38,38,38,0,13,4,4,0,0,4,4,4,0,0,0,38,38,38,38,0,13,13,0,0,4,24,24,24,4,0,0,38,38,38,13,13,13,33,0,4,24,9,24,24,24,4,0,0,13,13,13,13,13,4,4,2,2,9,9,24,24,4,0,0,13,36,13,13,2,4,4,1,2,9,13,4,4,0,0,0,13,13,13,13,2,4,4,4,4,9,36,9,0,0,0,0,38,38,38,13,13,4,4,1,2,9,36,9,9,0,0,38,38,38,38,4,4,4,4,2,2,9,13,9,9,0,0,38,38,38,13,4,4,4,4,4,4,9,9,9,0,0,0,0,13,13,13,13,13,4,4,4,24,24,24,24,4,0,0,0,0,0,13,13,13,4,4,24,24,24,24,24,24,4,0,0,0,0,0,0,0,0,4,4,24,24,24,24,24,4,0,0,0,0,0,0,0,0,0,0,4,24,24,24,4,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0},
        // dead
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,38,38,0,0,0,0,38,38,0,0,0,0,0,0,0,38,38,38,0,0,0,0,38,38,38,0,0,0,0,0,0,38,38,38,9,0,0,9,38,38,38,0,0,0,0,0,0,38,38,9,9,9,9,9,9,38,38,0,0,0,0,0,0,0,9,36,9,9,36,9,9,36,0,0,0,0,0,0,0,0,9,9,9,9,9,9,9,9,0,0,0,0,0,4,4,9,9,9,9,2,2,9,9,9,9,4,4,0,0,4,4,9,4,4,4,33,33,4,4,4,9,4,4,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,36,0,0,0,0,4,4,2,2,4,4,2,1,4,4,0,36,0,0,0,4,24,4,1,2,4,4,2,2,4,24,4,0,36,0,4,24,24,24,9,9,9,9,9,9,24,24,24,4,0,0,4,24,24,24,24,13,36,36,13,24,24,24,24,4,0,0,0,4,4,4,4,9,9,9,9,4,4,4,4,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,38,38,0,0,0,0,0,0,0,0,0,0,0,0,0,38,38,38,0,0,0,0,38,38,38,0,0,0,0,0,0,38,38,38,0,0,9,38,38,38,38,0,0,0,0,0,0,38,38,9,9,9,38,38,38,38,0,0,0,0,4,4,0,9,36,9,9,36,9,38,36,0,0,0,0,0,4,4,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,9,9,9,9,9,2,2,9,9,9,9,4,4,0,0,0,0,9,4,4,4,33,33,4,4,4,9,4,4,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,37,0,0,0,0,4,4,2,1,4,4,2,2,4,4,0,37,0,0,0,4,24,4,2,2,4,4,1,2,4,24,4,0,37,0,4,24,24,24,9,9,9,9,9,9,24,24,24,4,0,0,4,24,24,24,24,13,36,36,13,24,24,24,24,4,0,0,0,4,4,4,4,9,9,9,9,4,4,4,4,0,0,0},
        // goro 17
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        // mewkie 26
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,21,0,0,21,0,0,0,0,0,0,0,0,0,0,0,21,21,0,21,21,21,0,0,0,0,0,0,0,0,0,21,21,21,21,21,21,21,0,0,0,0,0,0,0,0,21,21,21,21,21,21,21,21,21,0,0,0,0,0,0,0,2,21,2,2,21,21,33,33,33,0,0,0,0,0,0,0,1,21,1,36,21,21,21,21,21,0,0,0,0,0,0,36,33,36,36,36,36,21,33,33,0,0,0,0,0,0,0,0,36,36,36,36,21,21,21,0,0,0,0,0,0,0,0,0,0,0,9,9,9,9,9,0,0,21,0,0,0,0,0,0,36,36,21,21,36,36,21,0,0,0,33,0,0,0,0,0,0,0,21,21,36,21,21,0,0,0,21,0,0,0,0,0,0,0,21,21,21,21,21,36,21,33,0,0,0,0,0,0,36,21,21,0,0,0,21,36,0,0,0,0,0,0,0,0,36,36,36,0,0,0,36,36,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,21,0,0,21,0,0,0,0,0,0,0,0,0,0,0,21,21,0,21,21,21,0,0,0,0,0,0,0,0,0,21,21,21,21,21,21,21,0,0,0,0,0,0,0,0,21,21,21,21,21,21,21,21,21,0,0,0,0,0,0,0,2,21,2,2,21,21,33,33,33,0,0,0,0,0,0,0,1,21,1,36,21,21,21,21,21,0,0,0,0,0,0,36,33,36,36,36,36,21,33,33,0,0,0,0,0,0,0,0,36,36,36,36,21,21,21,0,0,0,0,0,0,0,0,0,0,0,9,9,9,9,9,0,0,0,21,0,0,0,0,0,36,36,21,21,36,36,21,0,0,0,33,0,0,0,0,0,0,0,21,21,36,21,21,0,0,0,21,0,0,0,0,0,0,0,21,21,21,21,21,36,21,33,0,0,0,0,0,0,0,0,0,36,21,21,0,0,0,0,0,0,0,0,0,0,0,0,36,36,36,36,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,21,0,0,21,0,0,0,0,0,0,0,0,0,0,0,21,21,0,21,21,21,0,0,0,0,0,0,0,0,0,21,21,21,21,21,21,21,0,0,0,0,0,0,0,0,21,21,21,21,21,21,21,21,21,0,0,0,0,0,0,0,2,21,2,2,21,21,33,33,33,0,0,0,0,0,0,0,1,21,1,36,21,21,21,21,21,0,0,0,0,0,0,36,33,36,36,36,36,21,33,33,0,0,0,0,0,0,0,0,36,36,36,36,21,21,21,0,0,0,0,0,0,0,0,0,0,0,9,9,9,9,9,0,0,0,21,0,0,0,0,0,36,36,21,21,21,36,21,0,0,33,0,0,0,0,0,0,0,0,21,21,36,36,21,21,21,0,0,0,0,0,0,0,36,36,21,21,21,21,21,36,0,0,0,0,0,0,0,0,0,36,21,0,0,0,21,21,36,0,0,0,0,0,0,0,0,0,36,0,0,0,0,0,36,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,21,0,0,0,21,0,0,0,0,0,0,0,0,0,0,0,21,21,0,21,21,0,0,0,0,0,0,0,0,0,0,21,2,2,21,2,2,21,0,0,0,0,0,0,0,0,21,21,2,1,21,1,2,21,21,0,0,0,0,0,0,0,21,21,2,2,33,2,2,21,21,0,0,0,0,0,0,0,21,36,36,36,36,36,36,36,21,0,0,0,0,0,0,0,0,36,36,36,1,36,36,36,0,0,0,0,0,0,0,36,36,0,9,9,9,9,9,0,36,36,0,0,0,0,0,0,36,21,21,21,21,21,21,21,36,0,0,0,0,0,0,0,0,0,21,21,21,21,21,0,0,0,0,0,0,0,0,0,0,21,21,21,21,21,21,21,0,0,0,0,0,0,0,0,0,21,36,0,0,0,36,21,0,0,0,0,0,0,0,0,36,36,21,0,0,0,21,36,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,21,0,0,0,21,0,0,0,0,0,0,0,0,0,0,0,21,21,0,21,21,0,0,0,0,0,0,0,0,0,0,21,2,2,21,2,2,21,0,0,0,0,0,0,0,0,21,21,2,1,21,1,2,21,21,0,0,0,0,0,0,0,21,21,2,2,33,2,2,21,21,0,0,0,0,0,0,0,21,36,36,36,36,36,36,36,21,0,0,0,0,0,0,0,0,36,36,36,1,36,36,36,0,0,0,0,0,0,0,0,0,0,9,9,9,9,9,0,0,0,0,0,0,0,0,0,36,21,21,21,21,21,21,21,36,0,0,0,0,0,0,36,36,0,21,21,21,21,21,0,36,36,0,0,0,0,0,0,0,0,21,21,21,21,21,0,0,0,0,0,0,0,0,0,0,0,21,21,0,21,21,0,0,0,0,0,0,0,0,0,0,0,21,36,0,21,36,0,0,0,0,0,0,0,0,0,0,36,36,36,0,36,36,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,36,0,0,0,0,0,0,0,0,0,36,36,0,36,0,36,33,2,1,21,0,0,0,0,0,0,36,0,0,36,0,36,36,21,21,21,21,0,0,0,0,0,36,21,21,36,9,36,36,2,1,21,21,21,0,0,0,0,36,21,21,36,9,36,36,36,2,21,21,21,21,0,0,0,21,21,21,21,9,21,36,21,21,21,21,0,0,0,0,0,21,21,21,21,9,21,21,21,21,21,21,21,0,0,0,0,0,33,21,21,9,21,33,21,33,21,21,21,21,0,0,0,0,21,0,0,0,0,33,21,33,21,21,21,0,0,0,0,0,0,33,21,0,0,0,21,33,21,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        // trampoline
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,45,0,0,0,0,0,0,0,45,45,45,45,0,0,0,0,0,0,0,0,0,45,45,45,0,0,0,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,45,45,0,0,0,0,45,45,45,45,45,45,0,0,0,0,0,0,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    }},

    .objects = {{
        // trampoline
        {0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,0,0,45,45,45,0,0,0,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,0,0,0,0,0,0,0,0,45,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,0,0,0,0,0,0,0,0,45,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0,45,45,0,0,0,0,0,0,0,0},

        {0,45,45,45,45,45,45,0,45,0,0,0,0,0,0,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,45,45,45,45,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,45,45,45,45,45,45,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,0,0,0,0,0,0,45,0,45,45,45,45,45,45,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,0,0,0,0,0,0,45,0,45,45,45,45,45,45,0}
    }},

    .tiles = {{
        {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
        {37,37,37,37,37,37,37,37,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,37,37,37,37,37,37,37,37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,33,37,37,37,37,37,0,0,37,37,33,33,33,33,0,0,37,33,33,33,33,33,0,0,37,33,33,33,33,33,0,0,37,33,33,37,37,37,0,0,37,37,37,37,0,0,0,0,0,37,37,0,0,0,0,0,0,37,37,0,0,0},
        {37,37,37,37,37,37,37,37,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,37,37,37,33,33,37,37,37,0,0,37,37,37,37,0,0,0,0,0,37,37,0,0,0,0,0,0,37,37,0,0,0},
        {37,37,37,37,37,33,0,0,33,33,33,33,37,37,0,0,33,33,33,33,33,37,0,0,33,33,33,33,33,37,0,0,37,37,37,33,33,37,0,0,0,0,37,37,37,37,0,0,0,0,0,37,37,0,0,0,0,0,0,37,37,0,0,0},
        {37,37,37,37,37,37,37,37,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,37,37,33,33,33,33,37,37,37,37,37,33,33,37,37,37,0,0,37,37,37,37,0,0,0,0,37,37,37,37,0,0},

        {37,37,37,37,37,37,37,37,33,33,33,33,33,33,33,33,0,33,33,33,33,33,33,0,0,0,0,33,33,0,0,0,0,0,0,33,33,0,0,0,0,0,0,33,33,0,0,0,0,33,33,33,33,33,33,0,33,33,33,33,33,33,33,33},
        {37,37,37,37,37,33,0,0,33,33,33,33,37,37,0,0,0,33,33,33,33,37,0,0,0,0,0,33,33,37,0,0,0,0,0,33,33,37,0,0,0,0,0,33,33,37,0,0,0,33,33,33,37,37,0,0,33,33,33,33,37,33,0,0},
        {0,0,33,37,37,37,37,37,0,0,37,37,33,33,33,33,0,0,37,33,33,33,33,0,0,0,37,33,33,0,0,0,0,0,37,33,33,0,0,0,0,0,37,33,33,0,0,0,0,0,37,37,33,33,33,0,0,0,33,37,33,33,33,33},

        // 9 vertical
        {0,0,37,33,33,37,0,0,0,0,37,33,33,37,0,0,0,0,37,33,33,37,0,0,0,0,37,33,33,37,0,0,0,0,37,33,33,37,0,0,0,0,37,33,33,37,0,0,0,0,37,33,33,37,0,0,0,0,37,33,33,37,0,0},
        // 10 vertical right
        {37,37,37,37,37,37,0,0,33,33,33,33,33,37,0,0,33,33,33,33,33,37,0,0,33,33,33,33,33,37,0,0,37,37,37,33,33,37,0,0,0,0,37,33,33,37,0,0,0,0,37,33,33,37,0,0,0,0,37,33,33,37,0,0},
        // 11 vertical left
        {0,0,37,37,37,37,37,37,0,0,37,33,33,33,33,33,0,0,37,33,33,33,33,33,0,0,37,33,33,33,33,33,0,0,37,33,33,37,37,37,0,0,37,33,33,37,0,0,0,0,37,33,33,37,0,0,0,0,37,33,33,37,0,0},

        // 12 roof right
        {30,0,0,0,0,0,0,0,30,30,0,0,0,0,0,0,30,30,30,0,0,0,0,0,30,30,30,30,0,0,0,0,30,30,30,30,30,0,0,0,30,30,30,30,30,30,0,0,30,30,30,30,30,30,30,0,30,30,30,30,30,30,30,30},
        // 13 roof
        {30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30},
        // 14 roof left
        {0,0,0,0,0,0,0,30,0,0,0,0,0,0,30,30,0,0,0,0,0,30,30,30,0,0,0,0,30,30,30,30,0,0,0,30,30,30,30,30,0,0,30,30,30,30,30,30,0,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30}
    }},

    .levels ={{
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,14,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,0,0,0,0,
         0,0,0,14,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,0,0,0,
         0,0,14,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,0,0,
         0,14,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,0,
         14,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,
         11,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,10,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,
         7,0,0,0,8,6,6,6,6,6,6,6,6,6,6,6,6,7,0,0,0,8,6,6,6,6,6,6,6,6,6,6,6,6,7,0,0,0,8,6,6,6,6,6,6,6,6,6,6,6,6,7,0,0,0,8,
         7,0,0,0,8,6,6,6,6,6,6,6,6,6,6,6,6,7,0,0,0,8,6,6,6,6,6,6,6,6,6,6,6,6,7,0,0,0,8,6,6,6,6,6,6,6,6,6,6,6,6,7,0,0,0,8,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
         } //2,3,4,5,6,7,8,9,10,11,12,13,14}
    }},

    .interactive ={{
        {//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,
         1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
         1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
         }
    }}

    // .enemies = {{
    //     {},
    //     {},
    //     {},
    //     {},
    //     {},
    //     {},
    //     {},
    //     {},
    //     {}
    // }}
};