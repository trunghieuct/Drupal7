<?php

/**
 *  2009 Erick Romero (erm) <erick@adalid-soft.com>
 *  Gestation
*/

/**
 * @file
 * Admin page callbacks for the gestation module.
 */

function gestation_admin_page() {

  $output = '';

  $result = db_query('SELECT count(*) num_rows FROM {fertility}');
  if ( db_affected_rows()) {
    $node = db_fetch_object($result);
    $output .= sprintf('%d %s', $node->num_rows, t('emails stored'));
  }
  else {
    $output .= sprintf('<div> 0 %s </div> ', t('emails stored'));
  }

  $output .= sprintf('<div> %d %s </div> ', variable_get('gestation_counter', 0), t('gestation calculations have been processed'));

  $output .= sprintf('<br /><div> %s </div>', t('<a href="@link">Download emails</a>', array('@link' => url('admin/gestation/export'))));

  return $output;
}

function gestation_export ()  {

  @set_time_limit(600);
  $crlf="\n";

  $filename = 'Gestation Calculator';

  header('Content-type: application/octet-stream; charset=utf-8');
  header('Content-disposition: attachment; filename="'.$filename.'-'.date('Ymd').'.csv"');
  header('Cache-Control: public, must-revalidate');
  header('Pragma: hack');
  header('Content-Transfer-Encoding: binary');

  // doing some DOS-CRLF magic...
  $client = $_SERVER["HTTP_USER_AGENT"];
  if(ereg('[^(]*\((.*)\)[^)]*',$client,$regs)) {
  $os = $regs[1];
  // this looks better under WinX
  if (eregi("Win",$os))
      $crlf="\r\n";
  }

  $sql = "SELECT email, counter FROM {fertility} order by id";
  $result = db_query($sql);
  print "Email;Counter;" . $crlf;;
  while ($row = db_fetch_object($result)) {
    print $row->email.";";
    print $row->counter.";";
    print $crlf;
  }
  die();
}
