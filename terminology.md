See also: https://www.scratchapixel.com/lessons/3d-basic-rendering/3d-viewing-pinhole-camera/

* _pinhole camera_: a simple camera without a lens but with a tiny aperture (the so-called pinhole)—effectively a light-proof box with a small hole in one side. Light from a scene passes through the _aperture (center of projection)_ and projects an inverted image on the opposite side of the box, which is known as the _camera obscura_ effect. The size of the images depends on the distance between the object and the pinhole.

* _circle of confusion_: an optical spot caused by a cone of light rays from a lens not coming to a perfect focus when imaging a point source.

* _depth of field_: the distance between the nearest and the furthest objects that are in acceptably sharp focus in an image captured with a camera.

* _image plane_: that plane in the world which is identified with the plane of the display monitor used to view the image that is being rendered. It is also referred to as _screen space_ or _film_.

* _focal distance_: distance between pinhole and image plane. The further the image plane is from the pinhole, the bigger is the image (zoom out).

* _angle of view (field of view)_: angle at the apex of a triangle defined by the aperture and the film edges. The bigger the focal length, the smaller is the FOV. FOV is defined by either _vertical or horizontal FOV_, which connects aperture and top/bottom or left/right edges of the image plane.

* `Canvas size = 2 • tan(θ) • Distance to canvas` : control size of the objects in camera's view through angle of view.

* Size of the film matters. The smaller the surface, the smaller is the angle of view. If you use different film sizes but that your goal is to capture the exact same extent of a scene, you need to adjust the focal length.

* _film gate_: defines film aspect ratio, allows to change format without changing film or camera.

* _film aspect ratio_: ratio between physical width and height of the film.

* 


* _perspective projection_:
