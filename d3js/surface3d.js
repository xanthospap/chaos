 (function(){
  var Surface=function(node){
    console.log("called --Surface--");
    var heightFunction,colorFunction,timer,timer,transformPrecalc=[];
    var displayWidth=300, displayHeight=300, zoom=1;
    var trans;


    this.setZoom=function(zoomLevel){
      console.log("called surface::setZoom");
      zoom=zoomLevel;
      if(timer) clearTimeout(timer);
      timer=setTimeout(renderSurface);
    }; // this.setZoom

    var getHeights=function(opt_data){
      console.log("called surface::getHeights");
      var data;
      var heightFunction_tmp = heightFunction;
      if ( typeof opt_data == 'undefined' ) {
        data=node.datum();
      } else {
        console.log("FUCK GETHEIGHTS IN <MINE>");
        data=opt_data;
        heightFunction_tmp = function(t1,t2,t3) { return 0; }
      }
      var output=[];
      var xlength=data.length;
      var ylength=data[0].length;
      for(var x=0;x<xlength;x++){
        output.push(t=[]);
        for(var y=0;y<ylength;y++){
            var value=heightFunction_tmp(data[x][y],x,y);
            t.push(value);
        }
      }
      console.log("getHeights returned", output);
      return output;
    }; // getHeights

    var transformPoint=function(point){
      console.log("called surface::transformPoint");
      var x=transformPrecalc[0]*point[0]+transformPrecalc[1]*point[1]+transformPrecalc[2]*point[2];
      var y=transformPrecalc[3]*point[0]+transformPrecalc[4]*point[1]+transformPrecalc[5]*point[2];
      var z=transformPrecalc[6]*point[0]+transformPrecalc[7]*point[1]+transformPrecalc[8]*point[2];
      return [x,y,z];
    }; // transformPoint

    var getTransformedData=function(opt_data){
      console.log("called surface::getTransformedData");
      var data;
      if ( typeof opt_data == 'undefined' ) {
        data=node.datum();
      } else {
        console.log("calling with MYYYYYYYYYYY data");
        data=opt_data;
      }
      if(!heightFunction) return [[]];
      var t, output=[];
      var heights=getHeights(opt_data);
      var xlength=data.length;
      var ylength=data[0].length;
      for(var x=0;x<xlength;x++){
        output.push(t=[]);
        for(var y=0;y<ylength;y++){
          //console.log(heights, x, y);
          t.push(transformPoint([(x-xlength/2)/(xlength*1.41)*displayWidth*zoom,
                                 heights[x][y]*zoom,
                                 (y-ylength/2)/(ylength*1.41)*displayWidth*zoom]));
        }
      }
      return output;
    }; // getTransformedData

    var renderSurface=function(){
      console.log("called surface::renderSurface");
      var originalData=node.datum();
      var data=getTransformedData();
      var xlength=data.length;
      var ylength=data[0].length;
      var d0=[];
      var idx=0;
      //console.log("data has type: ", data);
      var max_x = -5000000, max_y = -5000000, max_d = -5000000;
      var min_x =  5000000, min_y =  5000000, min_d =  5000000;
      for(var x=0;x<xlength-1;x++){
        for(var y=0;y<ylength-1;y++){
          var depth=data[x][y][2]+data[x+1][y][2]+data[x+1][y+1][2]+data[x][y+1][2];
          if ( depth > max_d ) {
              max_d = depth;
          } else if ( depth < min_d ) {
              min_d = depth;
          }
          var mx  = (data[x][y][0]+displayWidth/2).toFixed(10);
          var my  = (data[x][y][1]+displayHeight/2).toFixed(10);
          var l1x = (data[x+1][y][0]+displayWidth/2).toFixed(10);
          var l1y = (data[x+1][y][1]+displayHeight/2).toFixed(10);
          var l2x = (data[x+1][y+1][0]+displayWidth/2).toFixed(10);
          var l2y = (data[x+1][y+1][1]+displayHeight/2).toFixed(10);
          var l3x = (data[x][y+1][0]+displayWidth/2).toFixed(10);
          var l3y = (data[x][y+1][1]+displayHeight/2).toFixed(10);
          if ( max_x < Math.max(mx, l1x, l2x, l3x) ) {
              max_x = Math.max(mx, l1x, l2x, l3x);
          }
          if ( min_x > Math.min(mx, l1x, l2x, l3x) ) {
              min_x = Math.min(mx, l1x, l2x, l3x);
          }
          if ( max_y < Math.max(my, l1y, l2y, l3y) ) {
              max_y = Math.max(my, l1y, l2y, l3y);
          }
          if ( min_y > Math.min(my, l1y, l2y, l3y) ) {
              min_y = Math.min(my, l1y, l2y, l3y);
          }
          d0.push({
            path:
              'M'+ mx  +',' + my  +
              'L'+ l1x +',' + l1y +
              'L'+ l2x +',' + l2y +
              'L'+ l3x +',' + l3y +
              'Z', depth: depth, data: originalData[x][y], ptxy: [mx,my,l1x,l1y,l2x,l2y,l3x,l3y]
          });
        }
      }
      var d1 = d0.slice(0);
      d0.sort(function(a, b){return b.depth-a.depth});
      var dlength = d0.length;
      var dr=node.selectAll('path').data(d0);
      dr.enter().append("path");
      if(trans){
        dr=dr.transition().delay(trans.delay()).duration(trans.duration());
      }
      dr.attr("d",function(d){return d.path;});
      if(colorFunction){
        dr.attr("fill",function(d){return colorFunction(d.data)});
      }

      var d1s = d1.length;
      console.log("d0 is:",d0);
      console.log("d1 is:",d1,d1s);
      //dr.enter().append("circle").attr("cx",d1[0].ptxy[0]).attr("cy",d1[0].ptxy[1]).attr("r","20");
      //dr.enter().append("circle").attr("cx",d1[d1s-1].ptxy[0]).attr("cy",d1[d1s-1].ptxy[1]).attr("r","5");
      //dr.enter().append("circle").attr("cx",d1[d1s-1].ptxy[2]).attr("cy",d1[d1s-1].ptxy[3]).attr("r","10");
      //dr.enter().append("circle").attr("cx",d1[d1s-1].ptxy[4]).attr("cy",d1[d1s-1].ptxy[5]).attr("r","15");
      //dr.enter().append("circle").attr("cx",d1[d1s-1].ptxy[6]).attr("cy",d1[d1s-1].ptxy[7]).attr("r","20");
      //dr.enter().append("circle").attr("cx",d1[11].ptxy[0]).attr("cy",d1[11].ptxy[1]).attr("r","5");
      //dr.enter().append("circle").attr("cx",d1[11].ptxy[2]).attr("cy",d1[11].ptxy[3]).attr("r","10");
      //dr.enter().append("circle").attr("cx",d1[11].ptxy[4]).attr("cy",d1[11].ptxy[5]).attr("r","15");
      //dr.enter().append("circle").attr("cx",d1[11].ptxy[6]).attr("cy",d1[11].ptxy[7]).attr("r","20");
      dr.enter().append("line")
                .attr("x1",d1[0].ptxy[0])
                .attr("y1",d1[0].ptxy[1])
                .attr("x2",d1[11].ptxy[6])
                .attr("y2",d1[11].ptxy[7])
                .attr("style","stroke:rgb(0,0,0);stroke-width:2");
      console.log("exiting surface::renderSurface");
      trans=false;
    }; // renderSurface

    this.renderSurface=renderSurface;

    this.setTurtable=function(yaw, pitch){
      console.log("called surface::setTurtable");
      var cosA=Math.cos(pitch);
      var sinA=Math.sin(pitch);
      var cosB=Math.cos(yaw);
      var sinB=Math.sin(yaw);
      transformPrecalc[0]=cosB;
      transformPrecalc[1]=0;
      transformPrecalc[2]=sinB;
      transformPrecalc[3]=sinA*sinB;
      transformPrecalc[4]=cosA;
      transformPrecalc[5]=-sinA*cosB;
      transformPrecalc[6]=-sinB*cosA;
      transformPrecalc[7]=sinA;
      transformPrecalc[8]=cosA*cosB;
      if(timer) clearTimeout(timer);
      timer=setTimeout(renderSurface);
      return this;
    }; // setTurtable

    this.setTurtable(0.5,0.5);

    this.surfaceColor=function(callback){
      console.log("called surface::surfaceColor");
      colorFunction=callback;
      if(timer) clearTimeout(timer);
      timer=setTimeout(renderSurface);
      return this;
    }; // surfaceColor

    this.surfaceHeight=function(callback){
      console.log("called surface::surfaceHeight");
      heightFunction=callback;
      if(timer) clearTimeout(timer);
      timer=setTimeout(renderSurface);
      return this;
    }; //surfaceHeight

    this.transition=function(){
      console.log("called surface::transition");
      var transition=d3.selection.prototype.transition.bind(node)();
      colourFunction=null;
      heightFunction=null;
      transition.surfaceHeight=this.surfaceHeight;
      transition.surfaceColor=this.surfaceColor;
      trans=transition;
      return transition;
    }; // transition

    this.setHeight=function(height){
      console.log("called surface::setHeight");
      if(height) displayHeight=height;
    }; // setHeight

    this.setWidth=function(width){
      console.log("called surface::setWidth");
      if(width) displayWidth=width;
    }; // setWidth

    console.log("exiting --Surface--");
  }; // var Surface=function(node)

  d3.selection.prototype.surface3D=function(width,height){
    console.log("calling __MAIN__");
    if(!this.node().__surface__) this.node().__surface__=new Surface(this);
    var surface=this.node().__surface__;
    this.turntable=surface.setTurtable;
    this.surfaceColor=surface.surfaceColor;
    this.surfaceHeight=surface.surfaceHeight;
    this.zoom=surface.setZoom;
    surface.setHeight(height);
    surface.setWidth(width);
    this.transition=surface.transition.bind(surface);
    console.log("out of __MAIN__");
    return this;
  };
})();
