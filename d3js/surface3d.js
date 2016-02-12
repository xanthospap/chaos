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
      for(var x=0;x<xlength-1;x++){
        for(var y=0;y<ylength-1;y++){
          var depth=data[x][y][2]+data[x+1][y][2]+data[x+1][y+1][2]+data[x][y+1][2];
          d0.push({
            path:
              'M'+(data[x][y][0]+displayWidth/2).toFixed(10)+','+(data[x][y][1]+displayHeight/2).toFixed(10)+
              'L'+(data[x+1][y][0]+displayWidth/2).toFixed(10)+','+(data[x+1][y][1]+displayHeight/2).toFixed(10)+
              'L'+(data[x+1][y+1][0]+displayWidth/2).toFixed(10)+','+(data[x+1][y+1][1]+displayHeight/2).toFixed(10)+
              'L'+(data[x][y+1][0]+displayWidth/2).toFixed(10)+','+(data[x][y+1][1]+displayHeight/2).toFixed(10)+
              'Z', depth: depth, data: originalData[x][y]
          });
        }
      }
      d0.sort(function(a, b){return b.depth-a.depth});
      var dr=node.selectAll('path').data(d0);
      dr.enter().append("path");
      if(trans){
        dr=dr.transition().delay(trans.delay()).duration(trans.duration());
      }
      dr.attr("d",function(d){return d.path;});
      if(colorFunction){
        dr.attr("fill",function(d){return colorFunction(d.data)});
      }
      var zen_axis = []
      for (var i=0;i<=90;i+=10) {
        zen_axis.push(t=[]);
        t.push(0);
      }
      var zenAxisData=getTransformedData(zen_axis);
      dr.enter().append("line")
                .attr("x1",(zenAxisData[x][y][0]+displayWidth/2).toFixed(10))
                .attr("y1",(zenAxisData[x][y][1]+displayWidth/2).toFixed(10))
                .attr("x2",0)
                .attr("y2","0")
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
