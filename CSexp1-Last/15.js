var space = 15;
var url;
var list = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15];

function game() {
	var cnt = 0;
	for(var i = 0; i < 16; i++) {
		if(cnt == list[i])cnt++;
	}
	if(cnt > 15) {
		result = alert("GAME CLEAR");
		$('#puzzle').fadeOut('slow', function() {$('#back-curtain').hide();});
	}
}

function onClick(e) {
	var canvas = document.getElementById('puzzle');
	var ctx = canvas.getContext('2d');
	var img = new Image();
	img.src = url;
	width = img.width/4;
	height = img.height/4;

	var cv = e.target.getBoundingClientRect();
	var x = Math.ceil((e.clientX - cv.left) / width - 1);
	var y = Math.ceil((e.clientY - cv.top) / height - 1);
	var sx = Math.ceil(space%4);
	var sy = Math.ceil(space/4-1);
	if(sx == 0)sy += 1;
	if(	(x - sx ==  1 && y - sy ==  0) || (x - sx == -1 && y - sy ==  0) ||
		(x - sx ==  0 && y - sy ==  1) || (x - sx ==  0 && y - sy == -1)	){
	ctx.drawImage(canvas,width*x,height*y,width,height,width*sx,height*sy,width,height);
	ctx.clearRect(width*x,height*y,width,height);	
		ctx.stroke();
		tmp = list[space];
		list[space] = list[x + y * 4];
		list[x + y * 4] = tmp;
		game();
		space = x + y * 4;
	}
}

function shuffle(u) {
	url = u;
	space = 15;
	var canvas = document.getElementById('puzzle');

	for(var i = 0; i < 16; i++) {
		list[i] = i;
	}

	for(var i = 0; i < 1000; i++) {
		r = [];
		if(space - 1 >= 0)r.push(space - 1);
		if(space + 1 < 16)r.push(space + 1);
		if(space - 4 >= 0)r.push(space - 4);
		if(space + 4 < 16)r.push(space + 4);
		next = r[Math.floor(Math.random() * r.length)];
		tmp = list[space];
		list[space] = list[next];
		list[next] = tmp;
		space = next;
	}

	if (canvas.getContext) {
		var ctx = canvas.getContext('2d'); 
		var img = new Image();
		ctx.clearRect(0,0,640,640);
		img.src = url;
    		img.onload = function() {
			var width = img.width/4;
			var height = img.height/4; 
			var cnt = 0;    

			for(var i = 0; i < 4; i++) {
				for(var j = 0; j < 4; j++) {
					sx = Math.ceil(list[cnt]%4);
					sy = Math.ceil(list[cnt]/4-1);
					if(sx == 0)sy += 1;
					if(cnt != space){
					cnt++;
					ctx.drawImage(img,width*sx,height*sy,width, height, width*j, height*i, width, height);
					}else cnt++;
				}
			}
			canvas.addEventListener('click',onClick,false);
			$('#back-curtain').css({
				'width' : $(window).width(),
				'height': $(window).height() * 100
			}).show();
			$('#puzzle').css({
				'position': 'absolute',
				'top': $(window).scrollTop() + 100 + 'px',
			}).fadeIn();
			$('#back-curtain').click(function() {
				$('#puzzle').fadeOut('fast', function() {$('#back-curtain').hide();});
			});
		}
	}
}
