package CBXWorkspaceCreator;

# ************************************************************
# Description   : A CBX Workspace Creator
# Author        : Chad Elliott
# Create Date   : 2/06/2004
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use CBXProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************


sub compare_output {
  #my($self) = shift;
  return 1;
}


sub workspace_file_name {
  my($self) = shift;
  return $self->get_modified_workspace_name($self->get_workspace_name(),
                                            '.bpgr');
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh '<?xml version="1.0" encoding="UTF-8"?>', $crlf,
            '<!-- CBXWorkspaceCreator.pm,v 1.4 2004/05/24 12:40:29 elliott_c Exp -->', $crlf,
            '<!-- MPC Command -->', $crlf,
            "<!-- $0 @ARGV -->", $crlf;
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my(@list)     = $self->sort_dependencies($projects);
  my($crlf)     = $self->crlf();

  print $fh '<!--Project Group-->', $crlf,
            '<projectgroup>', $crlf;
  foreach my $project (@list) {
    print $fh "  <project path=\"$project\"/>$crlf";
  }
  print $fh "</projectgroup>$crlf";
}


1;
