google.load("visualization", "1", {packages:["gauge"]});
google.setOnLoadCallback(drawChart);
//alert(Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.height);
function drawChart() {
  // simfps
  var cdata_simfps = new google.visualization.DataTable();
  cdata_simfps.addColumn('string', 'Label');
  cdata_simfps.addColumn('number', 'Value');
  cdata_simfps.addRows(1);
  cdata_simfps.setValue(0, 0, Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.name);
  cdata_simfps.setValue(0, 1, SimFPS);

  var chart_simfps = new google.visualization.Gauge(document.getElementById('simfps_chart_div'));
  var options_simfps = {
    width:      Number(Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.width),
    height:     Number(Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.height),
    min:        Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.min,
    max:        Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.max,
    minorTicks: Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.minorticks,
    greenFrom:  Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.greenfrom,
    greenTo:    Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.greento,
    yellowFrom: Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.yellowfrom,
    yellowTo:   Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.yellowto,
    redFrom:    Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.redfrom,
    redTo:      Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.simfps.redto
    };
  chart_simfps.draw(cdata_simfps, options_simfps);
  // phyfps
  var cdata_phyfps = new google.visualization.DataTable();
  cdata_phyfps.addColumn('string', 'Label');
  cdata_phyfps.addColumn('number', 'Value');
  cdata_phyfps.addRows(1);
  cdata_phyfps.setValue(0, 0, Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.name);
  cdata_phyfps.setValue(0, 1, PhyFPS);

  var chart_phyfps = new google.visualization.Gauge(document.getElementById('phyfps_chart_div'));
  var width = Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.width;
  var height = Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.height;
  var options_phyfps = {
    width:      Number(Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.width),
    height:     Number(Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.height),
    min:        Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.min,
    max:        Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.max,
    minorTicks: Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.minorticks,
    greenFrom:  Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.greenfrom,
    greenTo:    Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.greento,
    yellowFrom: Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.yellowfrom,
    yellowTo:   Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.yellowto,
    redFrom:    Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.redfrom,
    redTo:      Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.phyfps.redto
    };
  chart_phyfps.draw(cdata_phyfps, options_phyfps);
  // memory
  var cdata_memory = new google.visualization.DataTable();
  cdata_memory.addColumn('string', 'Label');
  cdata_memory.addColumn('number', 'Value');
  cdata_memory.addRows(1);
  cdata_memory.setValue(0, 0, Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.memory.name);
  cdata_memory.setValue(0, 1, Memory);

  var chart_memory = new google.visualization.Gauge(document.getElementById('memory_chart_div'));
  var options_memory = {
    width:      Number(Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.memory.width),
    height:     Number(Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.memory.height),
    min:        Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.memory.min,
    max:        Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.memory.max,
    minorTicks: Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.memory.minorticks,
    greenFrom:  Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.memory.greenfrom,
    greenTo:    Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.memory.greento,
    yellowFrom: Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.memory.yellowfrom,
    yellowTo:   Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.memory.yellowto,
    redFrom:    Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.memory.redfrom,
    redTo:      Drupal.settings.d4os_ui_grid_monitor_hosts_levels_widgets_gauges.params.memory.redto
    };
  chart_memory.draw(cdata_memory, options_memory);
}