var yaw=0.5,pitch=0.5, width=700, height=600, drag=false;
var hpad=50, wpad=50;

var svg=d3.select('body')
        .append('svg')
        .attr('height',height)
        .attr('width',width);

var group = svg.append("g");

// a scale to map pcv values to rgb (actually only r).
var domain = [-2.0, 2.0];
var range  = [0, 255];
var pcvScale = d3.scale.linear()
                   .domain(domain)
                   .range(range);

var zenScale = d3.scale.linear().domain([0,90]).range([0,90]);
var zenAxis  = d3.svg.axis().scale(zenScale).orient("bottom").ticks(5);

d3.json("leiatx.json", function(error, antex) {
if (error) throw error;

console.log("++ constructing Surface");
var md=group.data([antex.pcv_values])
          .surface3D(width,height)
          .surfaceHeight(function(d){
              console.log("got height:",d);
               return d*100;
          })
          .surfaceColor(function(d){
              var c=d3.hsl(pcvScale(d), 0.6, 0.5).rgb();
              return "rgb("+parseInt(c.r)+","+parseInt(c.g)+","+parseInt(c.b)+")";
          });
console.log("++ all done!");

svg.on("mousedown",function(){
drag=[d3.mouse(this),yaw,pitch];
}).on("mouseup",function(){
drag=false;
}).on("mousemove",function(){
if(drag){
  var mouse=d3.mouse(this);
  yaw=drag[1]-(mouse[0]-drag[0][0])/50;
  pitch=drag[2]+(mouse[1]-drag[0][1])/50;
  pitch=Math.max(-Math.PI/2,Math.min(Math.PI/2,pitch));
  md.turntable(yaw,pitch);
}
});

});
