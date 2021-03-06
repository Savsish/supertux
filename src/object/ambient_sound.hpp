//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

/**
 *  Ambient Sound Source, gamma version. Features:
 *
 *  - "rounded rectangle" geometry with position, dimension and
 *    "rounding radius" (extending in all directions) of a 100%
 *    volume area, adjustable maximum volume, inverse square
 *    falloff outside area.
 *
 *  - degenerates gracefully to a disc for dimension=0
 *
 *  - parameters:
 *
 *    x, y               position
 *    width, height      dimension
 *    distance_factor    high = steep falloff
 *    distance_bias      high = big "100% disc"
 *    silence_distance   defaults reasonably.
 *    sample             sample to be played back in loop mode
 *
 *      basti_
 */

#ifndef HEADER_SUPERTUX_OBJECT_AMBIENT_SOUND_HPP
#define HEADER_SUPERTUX_OBJECT_AMBIENT_SOUND_HPP

#include "math/vector.hpp"
#include "supertux/moving_object.hpp"
#include "supertux/script_interface.hpp"

class GameObject;
class Player;
class ReaderMapping;
class SoundSource;

class AmbientSound : public MovingObject,
                     public ScriptInterface
{
public:
  AmbientSound(const ReaderMapping& lisp);
  AmbientSound(Vector pos, float factor, float bias, float vol, std::string file);
  ~AmbientSound();

  HitResponse collision(GameObject& other, const CollisionHit& hit_);

  const Vector get_pos() const
  {
    return bbox.p1;
  }
  std::string get_class() const {
    return "ambient_sound";
  }
  /**
   * @name Scriptable Methods
   * @{
   */
#ifndef SCRIPTING_API
  void set_pos(const Vector& pos);
#endif
  void set_pos(float x, float y);
  float get_pos_x() const;
  float get_pos_y() const;
  /**
   * @}
   */

  void draw(DrawingContext& context);

  std::string get_display_name() const {
    return _("Ambient sound");
  }
  virtual ObjectSettings get_settings();
  virtual void after_editor_set();

protected:
  virtual void hit(Player& player);
  virtual void update(float time);
  virtual void start_playing();
  virtual void stop_playing();
  virtual void expose(HSQUIRRELVM vm, SQInteger table_idx);
  virtual void unexpose(HSQUIRRELVM vm, SQInteger table_idx);

private:

  std::string sample;
  std::unique_ptr<SoundSource> sound_source;
  int latency;

  float distance_factor;  /// distance scaling
  float distance_bias;    /// 100% volume disc radius
  float silence_distance; /// not implemented yet

  float maximumvolume; /// maximum volume
  float targetvolume;  /// how loud we want to be
  float currentvolume; /// how loud we are

  float * volume_ptr; /// this will be used by the volume adjustment effect.

  Vector new_size;

private:
  AmbientSound(const AmbientSound&);
  AmbientSound& operator=(const AmbientSound&);
};

#endif

/* EOF */
