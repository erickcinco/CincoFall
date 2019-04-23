from PIL import Image
from PIL import GifImagePlugin
import os

def resize_with_aspect_ratio(basewidth, img, name):

    wpercent = (basewidth / float(img.size[0]))
    hsize = int((float(img.size[1]) * float(wpercent)))
    img_resize = img.resize((basewidth, hsize), Image.ANTIALIAS)
    # print img_resize.size
    img_resize.save('{}_resized_lcd.bmp'.format(name))
    return img_resize


def rgb_hex565_string(red, green, blue):
    b = (blue >> 3) & 0x1f;
    g = ((green >> 2) & 0x3f) << 5;
    r = ((red >> 3) & 0x1f) << 11;

    return format(r|g|b, '#04x');


def write_sprite_header(sprite_name, basewidth):
    img = Image.open('{}.png'.format(sprite_name))
    image_resize = resize_with_aspect_ratio(basewidth, img, sprite_name)

    array_name = sprite_name + '_color_array'
    c_name = array_name + '.h'

    sprite = image_resize
    color_array = list(sprite.getdata())
    # print color_array
    first_line = 'uint16_t {}[{}_SPRITE_SIZE] = {{'.format( array_name, sprite_name.upper())

    c_file = open(c_name, 'w')
    c_file.write('#ifndef {}_COLOR_ARRAY_H_\n#define {}_COLOR_ARRAY_H_\n\n'.format(sprite_name.upper(), sprite_name.upper(), ))
    c_file.write('#define {}_SPRITE_SIZE {}\n'.format(sprite_name.upper(), len(color_array)))
    c_file.write('#define {}_SPRITE_WIDTH {}\n'.format(sprite_name.upper(), sprite.size[0]))
    c_file.write('#define {}_SPRITE_HEIGHT {}\n\n'.format(sprite_name.upper(), sprite.size[1]))

    c_file.write(first_line)
    offset = len(first_line)
    i = 0
    for color in color_array:
        color_hex565 = rgb_hex565_string(color[0], color[1], color[2])
        c_file.write('{}'.format(color_hex565))
        # print (color_hex565, i, len(color_array))
        if i != len(color_array)-1:
            c_file.write(', ')

        if not i % 10 and i != 0 and i != len(color_array)-1:
            c_file.write('\n')
            for j in range(offset):
                c_file.write(' ')

        i+=1

    c_file.write('};')
    c_file.write('\n\r#endif /* {}_COLOR_ARRAY_H_ */\n'.format(sprite_name.upper()))
    c_file.close()

# write_sprite_header('cat', 20)
# write_sprite_header('parrot', 20)
# write_sprite_header('dog', 20)
# write_sprite_header('cat_fighter', 40)
# write_map_header('cataclysm', 320, 'cataclysm_map.jpg')






def gif_to_sprite_header(file_name, sprite_name, basewidth, crop=None):

    imageObject = Image.open(file_name)

    c_name = sprite_name + '_gif_color_array' + '.h'
    c_file = open(c_name, 'w')
    c_file.write('#ifndef {}_COLOR_ARRAY_H_\n#define {}_COLOR_ARRAY_H_\n\n'.format(sprite_name.upper(), sprite_name.upper(), ))

    for frame in range(0,imageObject.n_frames): # TODO get rid of this 10
        imageObject.seek(frame)
        if crop:
            image_colored = imageObject.crop((19, 20, imageObject.size[0]-15, imageObject.size[1]-10))
            image_colored = image_colored.convert('RGB')
        else:
            image_colored = imageObject.convert('RGB')

        # image_colored.show()
        sprite = resize_with_aspect_ratio(basewidth, image_colored, sprite_name)
        # sprite.show()
        array_name = sprite_name + '_gif_color_array_frame_{}'.format(frame)
        color_array = list(sprite.getdata())

        if(frame == 0):
            c_file.write('#define {}_SPRITE_SIZE {}\n'.format(sprite_name.upper(), len(color_array)))
            c_file.write('#define {}_SPRITE_WIDTH {}\n'.format(sprite_name.upper(), sprite.size[0]))
            c_file.write('#define {}_SPRITE_HEIGHT {}\n\n'.format(sprite_name.upper(), sprite.size[1]))

        # print color_array
        first_line = 'uint16_t {}[{}_SPRITE_SIZE] = {{'.format(array_name, sprite_name.upper())

        c_file.write(first_line)
        offset = len(first_line)
        i = 0
        for color in color_array:
            color_hex565 = rgb_hex565_string(color[0], color[1], color[2])
            c_file.write('{}'.format(color_hex565))
            if i != len(color_array)-1:
                c_file.write(', ')

            if not i % 10 and i != 0 and i != len(color_array)-1:
                c_file.write('\n')
                for j in range(offset):
                    c_file.write(' ')

            i+=1

        c_file.write('};\n\n')
    c_file.write('\n\r#endif /* {}_COLOR_ARRAY_H_ */\n'.format(sprite_name.upper()))
    c_file.close()


# gif_to_sprite_header("./cat_addon_energyforcemaster/cat_a1_super.gif", "fighter_cat", 40, crop=True)
gif_to_sprite_header("./cat_addon_energyforcemaster/cat_a1_super.gif", "fighter_cat", 25, crop=True)

