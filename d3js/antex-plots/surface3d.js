 (function(){
  var Surface=function(node){
    var heightFunction,colorFunction,timer,timer,transformPrecalc=[];
    var displayWidth=500, displayHeight=500, zoom=1.0;
    var trans;


    this.setZoom=function(zoomLevel){
      zoom=zoomLevel;
      if(timer) clearTimeout(timer);
      timer=setTimeout(renderSurface);
    }; // this.setZoom

    var getHeights=function(/*opt_data*/){
      var data;
      data=node.datum();
      var output=[];
      var xlength=data.length;
      var ylength=data[0].length;
      for(var x=0;x<xlength;x++){
        output.push(t=[]);
        for(var y=0;y<ylength;y++){
            var value=heightFunction(data[x][y],x,y);
            console.log("height of",data[x][y],x,y,"is",value);
            t.push(value);
        }
      }
      return output;
    }; // getHeights

    var transformPoint=function(point){
      var x=transformPrecalc[0]*point[0]+transformPrecalc[1]*point[1]+transformPrecalc[2]*point[2];
      var y=transformPrecalc[3]*point[0]+transformPrecalc[4]*point[1]+transformPrecalc[5]*point[2];
      var z=transformPrecalc[6]*point[0]+transformPrecalc[7]*point[1]+transformPrecalc[8]*point[2];
      return [x,y,z];
    }; // transformPoint

    var getTransformedDataPoint=function(x,y,h,xl,yl) {
        return transformPoint([(x-xl/2)/(xl*1.41)*displayWidth*zoom,
                                h*zoom,
                                (y-yl/2)/(yl*1.41)*displayWidth*zoom]);
    };

    var getTransformedData=function(/*opt_data*/){
      var data;
      data=node.datum();
      var t, output=[];
      var heights=getHeights(data);
      var xlength=data.length;
      var ylength=data[0].length;
      for(var x=0;x<xlength;x++){
        output.push(t=[]);
        for(var y=0;y<ylength;y++){
          t.push(transformPoint([(x-xlength/2)/(xlength*1.41)*displayWidth*zoom,
                                 heights[x][y]*zoom,
                                 (y-ylength/2)/(ylength*1.41)*displayWidth*zoom]));
        }
      }
      return output;
    }; // getTransformedData


    var renderSurface=function(){
      var originalData=node.datum();
      var data=getTransformedData();
      var xlength=data.length;
      var ylength=data[0].length;
      var d0=[];
      var idx=0;
      var y_max = -50000;
      var y_min =  50000;
      for(var x=0;x<xlength-1;x++){
        for(var y=0;y<ylength-1;y++){
          var depth=data[x][y][2]+data[x+1][y][2]+data[x+1][y+1][2]+data[x][y+1][2];
          var mx  = (data[x][y][0]+displayWidth/2).toFixed(10);
          var my  = (data[x][y][1]+displayHeight/2).toFixed(10);
          var l1x = (data[x+1][y][0]+displayWidth/2).toFixed(10);
          var l1y = (data[x+1][y][1]+displayHeight/2).toFixed(10);
          var l2x = (data[x+1][y+1][0]+displayWidth/2).toFixed(10);
          var l2y = (data[x+1][y+1][1]+displayHeight/2).toFixed(10);
          var l3x = (data[x][y+1][0]+displayWidth/2).toFixed(10);
          var l3y = (data[x][y+1][1]+displayHeight/2).toFixed(10);
          if ( y_max < my ) y_max = my;
          d0.push({
            path:
              'M'+ mx  +',' + my  +
              'L'+ l1x +',' + l1y +
              'L'+ l2x +',' + l2y +
              'L'+ l3x +',' + l3y +
              'Z', depth: depth, data: originalData[x][y]/*, ptxy: [mx,my,l1x,l1y,l2x,l2y,l3x,l3y]*/
          });
        }
      }
      d0.sort(function(a, b){return b.depth-a.depth});
      var dlength = d0.length;
      var dr=node.selectAll('path').data(d0);
      dr.enter().append("path");
      if(trans) {
        dr=dr.transition().delay(trans.delay()).duration(trans.duration());
      }
      dr.attr("d",function(d){return d.path;});
      if(colorFunction){
        dr.attr("fill",function(d){return colorFunction(d.data)});
      }
      var origin = getTransformedDataPoint(0,0,0,xlength,1);
      var y_end  = getTransformedDataPoint(0,ylength-1,0,xlength,ylength);
      var x_end  = getTransformedDataPoint(xlength-1,0,0,xlength,1);
      console.log(origin);
      dr.enter().append("circle")
                .attr("cx",(origin[0]+displayWidth/2).toFixed(10))
                .attr("cy",(origin[1]+displayHeight/2).toFixed(10))
                .attr("r","5");
      console.log(x_end);
      dr.enter().append("circle")
                .attr("cx",(x_end[0]+displayWidth/2).toFixed(10))
                .attr("cy",(x_end[1]+displayHeight/2).toFixed(10))
                .attr("r","5");
      dr.enter().append("circle")
                .attr("cx",(y_end[0]+displayWidth/2).toFixed(10))
                .attr("cy",(y_end[1]+displayHeight/2).toFixed(10))
                .attr("r","5");
      dr.enter().append("circle")
                .attr("cx",(origin[0]+displayWidth/2).toFixed(10))
                .attr("cy",y_max)
                .attr("r","5");
      dr.enter().append("line") /* x-axis */
                .attr("x1",(origin[0]+displayWidth/2).toFixed(10))
                .attr("y1",(origin[1]+displayHeight/2).toFixed(10))
                .attr("x2",(x_end[0]+displayWidth/2).toFixed(10))
                .attr("y2",(x_end[1]+displayHeight/2).toFixed(10))
                .attr("style","stroke:rgb(0,0,0);stroke-width:2");
      dr.enter().append("line") /* y-axis */
                .attr("x1",(origin[0]+displayWidth/2).toFixed(10))
                .attr("y1",(origin[1]+displayHeight/2).toFixed(10))
                .attr("x2",(y_end[0]+displayWidth/2).toFixed(10))
                .attr("y2",(y_end[1]+displayHeight/2).toFixed(10))
                .attr("style","stroke:rgb(0,0,0);stroke-width:2");
      dr.enter().append("line") /* z-axis */
                .attr("x1",(origin[0]+displayWidth/2).toFixed(10))
                .attr("y1",(origin[1]+displayHeight/2).toFixed(10))
                .attr("x2",(origin[0]+displayWidth/2).toFixed(10))
                .attr("y2",y_max)
                .attr("style","stroke:rgb(0,0,0);stroke-width:2");

      trans=false;
    }; // renderSurface
    this.renderSurface=renderSurface;

    this.setTurtable=function(yaw, pitch){
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
      colorFunction=callback;
      if(timer) clearTimeout(timer);
      timer=setTimeout(renderSurface);
      return this;
    }; // surfaceColor

    this.surfaceHeight=function(callback){
      heightFunction=callback;
      if(timer) clearTimeout(timer);
      timer=setTimeout(renderSurface);
      return this;
    }; //surfaceHeight

    this.transition=function(){
      var transition=d3.selection.prototype.transition.bind(node)();
      colourFunction=null;
      heightFunction=null;
      transition.surfaceHeight=this.surfaceHeight;
      transition.surfaceColor=this.surfaceColor;
      trans=transition;
      return transition;
    }; // transition

    this.setHeight=function(height){
      if(height) displayHeight=height;
    }; // setHeight

    this.setWidth=function(width){
      if(width) displayWidth=width;
    }; // setWidth

  }; // var Surface=function(node)

  d3.selection.prototype.surface3D=function(width,height){
    if(!this.node().__surface__) this.node().__surface__=new Surface(this);
    var surface=this.node().__surface__;
    this.turntable=surface.setTurtable;
    this.surfaceColor=surface.surfaceColor;
    this.surfaceHeight=surface.surfaceHeight;
    this.zoom=surface.setZoom;
    surface.setHeight(height-200);
    surface.setWidth(width);
    this.transition=surface.transition.bind(surface);
    return this;
  };
})();
