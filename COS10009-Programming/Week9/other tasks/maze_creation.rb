require 'gosu'

module ZOrder
  BACKGROUND, MIDDLE, TOP = *0..2
end

MAP_WIDTH = 200
MAP_HEIGHT = 200
CELL_DIM = 20

class Cell
  # have a pointer to the neighbouring cells
  def initialize
    # Set the pointers to nil
    @north = nil
    @south = nil
    @east = nil
    @west = nil
    # default is not vacant i.e is a wall.
    @vacant = false
    # this stops cycles - set when you travel through a cell
    @visited = false
    @on_path = false
  end

  attr_accessor :north, :south, :east, :west, :vacant, :visited, :on_path
end

# Instructions:
# Left click on cells to create a maze with at least one path moving from
# left to right. The right click on a cell for the program to find a path
# through the maze. When a path is found it will be displayed in red.
class GameWindow < Gosu::Window
  # initialize creates a window with a width an a height
  # and a caption. It also sets up any variables to be used.
  # This is procedure i.e the return value is 'undefined'
  def initialize
    super MAP_WIDTH, MAP_HEIGHT, false
    self.caption = 'Map Creation'

    @path = nil

    x_cell_count = MAP_WIDTH / CELL_DIM
    y_cell_count = MAP_HEIGHT / CELL_DIM
    @columns = Array.new(x_cell_count)

    create_cell_for_each_position(x_cell_count, y_cell_count)
    set_up_neighbour_links(x_cell_count, y_cell_count)
  end

  def create_cell_for_each_position(x_cell_count, y_cell_count)
    column_index = 0
    while column_index < x_cell_count
      row = Array.new(y_cell_count)
      @columns[column_index] = row
      row_index = 0
      while row_index < y_cell_count
        cell = Cell.new
        @columns[column_index][row_index] = cell
        row_index += 1
      end
      column_index += 1
    end
  end

  def set_up_neighbour_links(x_cell_count, y_cell_count)
    column_index = 0
    while column_index < x_cell_count
      row_index = 0
      while row_index < y_cell_count
        cell = @columns[column_index][row_index]
        cell.north = @columns[column_index][row_index - 1] if row_index > 0
        cell.south = @columns[column_index][row_index + 1] if row_index < y_cell_count - 1
        cell.west = @columns[column_index - 1][row_index] if column_index > 0
        cell.east = @columns[column_index + 1][row_index] if column_index < x_cell_count - 1
        row_index += 1
      end
      column_index += 1
    end
  end


  # this is called by Gosu to see if should show the cursor (or mouse)
  def needs_cursor?
    true
  end

  # Returns an array of the cell x and y coordinates that were clicked on
  def mouse_over_cell(mouse_x, mouse_y)
    cell_x = mouse_x <= CELL_DIM ? 0 : (mouse_x / CELL_DIM).to_i
    cell_y = mouse_y <= CELL_DIM ? 0 : (mouse_y / CELL_DIM).to_i
    [cell_x, cell_y]
  end

  # start a recursive search for paths from the selected cell
  # it searches till it hits the East 'wall' then stops
  # it does not necessarily find the shortest path

  def search(cell_x, cell_y)
    # dead_end = false
    # path_found = false
    if cell_x == MAP_WIDTH / CELL_DIM - 1
      puts 'End of one path x: ' + cell_x.to_s + ' y: ' + cell_y.to_s if ARGV.length > 0 # debug
      [[cell_x, cell_y]] # We are at the east wall - exit
    else

      north_path = nil
      west_path = nil
      east_path = nil
      south_path = nil
      puts 'Searching. In cell x: ' + cell_x.to_s + ' y: ' + cell_y.to_s if ARGV.length > 0 # debug

      # INSERT MISSING CODE HERE!! You need to have 4 'if' tests to
      # check each surrounding cell. Make use of the attributes for
      # cells such as vacant, visited and on_path.
      # Cells on the outer boundaries will always have a nil on the
      # boundary side
      cell = @columns[cell_x][cell_y]
      cell.visited = true
      if cell.east && cell.east.vacant && (!cell.east.visited || cell.east.on_path)
        east_path = [cell_x + 1, cell_y]
      elsif cell.south && cell.south.vacant && (!cell.south.visited || cell.south.on_path)
        south_path = [cell_x, cell_y + 1]
      elsif cell.north && cell.north.vacant && (!cell.north.visited || cell.north.on_path)
        north_path = [cell_x, cell_y - 1]
      elsif cell.west && cell.west.vacant && (!cell.west.visited || cell.west.on_path)
        west_path = [cell_x - 1, cell_y]
      end
      # pick one of the possible paths that is not nil (if any):

      if north_path
        path = north_path
      elsif south_path
        path = south_path
      elsif east_path
        path = east_path
      elsif west_path
        path = west_path
      end
      # dead end
      return nil unless path

      # A path was found:
      next_search = search(path[0], path[1])
      return [[cell_x, cell_y]].concat(next_search) if next_search

      search(cell_x, cell_y)
    end
  end

  # Reacts to button press
  # left button marks a cell vacant
  # Right button starts a path search from the clicked cell
  def button_down(id)
    cell = mouse_over_cell(mouse_x, mouse_y)
    case id
    when Gosu::MsLeft
      puts('Cell clicked on is x: ' + cell[0].to_s + ' y: ' + cell[1].to_s) if ARGV.length > 0 # debug
      @columns[cell[0]][cell[1]].vacant = true
    when Gosu::MsRight
      @path = search(cell[0], cell[1])
    end
  end

  # This will walk along the path setting the on_path for each cell
  # to true. Then draw checks this and displays them a red colour.
  def walk(path)
    count = 0
    while count < path.length
      cell = path[count]
      @columns[cell[0]][cell[1]].on_path = true
      count += 1
    end
  end

  # Put any work you want done in update
  # This is a procedure i.e the return value is 'undefined'
  def update
    return unless @path

    if ARGV.length > 0 # debug
      puts 'Displaying path'
      puts @path
    end
    walk(@path)
    @path = nil
  end

  # Draw (or Redraw) the window
  # This is procedure i.e the return value is 'undefined'
  def draw
    index = 0
    x_loc = 0
    y_loc = 0

    x_cell_count = MAP_WIDTH / CELL_DIM
    y_cell_count = MAP_HEIGHT / CELL_DIM

    column_index = 0
    while column_index < x_cell_count
      row_index = 0
      while row_index < y_cell_count

        color = @columns[column_index][row_index].vacant ? Gosu::Color::YELLOW : Gosu::Color::GREEN
        color = Gosu::Color::RED if @columns[column_index][row_index].on_path

        Gosu.draw_rect(column_index * CELL_DIM, row_index * CELL_DIM, CELL_DIM, CELL_DIM, color, ZOrder::TOP,
                       mode = :default)

        row_index += 1
      end
      column_index += 1
    end
  end
end

window = GameWindow.new
window.show
