var SimFPS = 0;
var PhyFPS = 0;
var Memory = 0;
var RootAg = 0;
var ChldAg = 0;
var Uptime = "";
var Version = "";
var levels_timer = Drupal.settings.d4os_ui_grid_monitor_hosts_levels.timer*1000;
var host = Drupal.settings.d4os_ui_grid_monitor_hosts_levels.host;
var port = Drupal.settings.d4os_ui_grid_monitor_hosts_levels.port;
var path = Drupal.settings.d4os_ui_grid_monitor_hosts_levels.path;

function reqJSON(url, params, success, error) {
  ///<summary>Make a JSON call</summary>
  ///<param name="url" type="String" optional="false">Url of handler to call</param>
  ///<param name="params" type="Object" optional="false">params to pass to handler,
  ///this event looks for ".Method" (GET/POST) as well</param>
  ///<param name="success" type="Object" optional="false">Event to call when complete</param>
  ///<param name="error" type="Object" optional="true">Event to call when an error is encountered</param>
  var CallParams = {};
  CallParams.type = params.Method || "GET";
  CallParams.url = url;
  CallParams.processData = true;
  CallParams.data = params;
  CallParams.dataType = "json";
  CallParams.success = success;
  if (error) {
      CallParams.error = error;
  }
  $.ajax(CallParams);
}

function jsonSuccess(data, timer) {
  if (data == 'error') {
    $('#d4os-stats-levels').hide();
    $('#d4os-stats-levels-error').show();
  }
  else {
    $('#d4os-stats-levels').show();
    $('#d4os-stats-levels-error').hide();
    SimFPS = Math.round(data.SimFPS);
    PhyFPS = Math.round(data.PhyFPS);
    Memory = Math.round(data.Memory);
    ChldAg = data.ChldAg;
    RootAg = data.RootAg;
    Uptime = data.Uptime;
    Version = data.Version;
    drawChart();
    setTags();
  }
  timer.reset(levels_timer);
}

function jsonError(timer) {
  $('#d4os-stats-levels').hide();
  $('#d4os-stats-levels-error').show();
  timer.reset(levels_timer);
}

// get the data
$.timer(levels_timer, function(timer) {
  reqJSON('/grid/monitor/hosts/levels/js/'+ host+ '/'+ port, {},
    function (data) {
      jsonSuccess(data, timer);
    });//,jsonError(timer));
});
