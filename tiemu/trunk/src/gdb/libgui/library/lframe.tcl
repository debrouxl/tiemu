# lframe.tcl - Labelled frame widget.
# Copyright (C) 1997 Cygnus Solutions.
# Written by Tom Tromey <tromey@cygnus.com>.

itcl::class Labelledframe {
  inherit Widgetframe

  # The label text.
  public variable text {} {
    if {[winfo exists [namespace tail $this].label]} then {
      [namespace tail $this].label configure -text $text
    }
  }

  constructor {args} {
    eval configure $args
    label [namespace tail $this].label -text $text -padx 2
    _add [namespace tail $this].label
  }
}
