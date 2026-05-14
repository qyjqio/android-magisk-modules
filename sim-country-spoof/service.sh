#!/system/bin/sh
# SIM Country Spoof - bypass TikTok regional restriction
# Loops every 10s to fight telephony service overwriting SIM props

while true; do
  sim_op=$(getprop gsm.sim.operator.numeric)
  case "$sim_op" in
    460*)  resetprop gsm.sim.operator.numeric 310260;;
  esac

  net_op=$(getprop gsm.operator.numeric)
  case "$net_op" in
    460*)  resetprop gsm.operator.numeric 310260;;
  esac

  iso_co=$(getprop gsm.sim.operator.iso-country)
  case "$iso_co" in
    *,cn*|cn,*) resetprop gsm.sim.operator.iso-country us;;
  esac

  sleep 10
done
