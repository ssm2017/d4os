var simfps_chart_div = $("#simfps_chart_div");
var phyfps_chart_div = $("#phyfps_chart_div");
var memory_chart_div = $("#memory_chart_div");

function drawChart() {
  simfps_chart_div.drawBar({value:SimFPS, name:"Sim FPS", suffix:"FPS"});
  phyfps_chart_div.drawBar({value:PhyFPS, name:"Phy FPS", suffix:"FPS"});
  memory_chart_div.drawBar({value:Memory, name:"Memory", suffix:"Mo"});
}

function setTags() {
  $("#par-uptime").text(Uptime);
  $("#par-ragent").text(RootAg);
  $("#par-version").text(Version);
  $("#par-cagent").text(ChldAg);
}

jQuery.fn.drawBar = function(options) {

  var o = jQuery.extend({
    value: "100",
    name: "Value",
    size: "32",
    direction: "h"
  }, options);

  // check if the bar exists
  var pb = this.find('.progress-bar');
  if (pb.length != 1) {
    buildBar(this, o);
  }

  // remove all the classes
  pb.removeClass();

  // define the color for the background
  var color = "green";

  // build the image class
  var img_class = o.direction+ '-'+ o.size+ '-'+ color;

  // add the background image class
  pb.addClass('progress-bar '+ img_class);

  // animate
  pb.animate({width: o.value},{
    duration: 'slow',
    easing: 'easeOutBack'
  });

  // change text
  //this.find('.progress-bar-text').text(o.width+ ' '+ o.suffix);

  function buildBar(el, o) {
    el.addClass('progress-bar-wrapper');
    el.append('<label>'+ o.name+ '</label><div class="progress-bar-text"><div class="progress-bar init"></div></div>');
    el.drawBar(o);
  }
  return this;
};
$(document).ready(function(){
  drawChart();
});