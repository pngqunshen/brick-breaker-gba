@ limit angle to [-180, 180)
.global limit_angle
limit_angle:
    @ r0: angle
    push {lr}           @ save the link register

loop:
    cmp   r0, #-180     @ if (a < -180)
    addlt r0, r0, #360  @ a += 360
    blt   loop          @ loop
    cmp   r0, #180      @ if (a >= 180)
    subge r0, r0, #360  @ a -= 360
    bge   loop          @ loop

    @ Exit from function
    pop {pc}            @ restore the link register and return to the caller

.global platform_top_deflection
platform_top_deflection:
    @ r0: ball heading
    @ r1: ball center x coordinate
    @ r2: platform x coordinate
    push {lr}           @ save the link register
    stmfd   sp!, {r4-r11, lr}     @ save content of r4-r11 and link register into the sp register

    add   r3, r2, #16      @ r3 = platform_x + 16
    sub   r4, r2, #16      @ r4 = platform_x - 16
    cmp   r1, r3           @ if (ball_x < r3)
    bge   end              @ else, no reflect
    cmp   r1, r4           @ if (ball_x >= r4)
    bge   reflect          @ both conditions satisfy, reflect
    blt   end              @ else, no reflect

reflect:
    @ find additional angle to deflect ball at
    @ PLATFORM_MAX_DEFLECTION: 32   , maximum additional deflection by hitting end of platform
    @ PLATFORM_MIN_ANGLE: -160      , minimum allowed angle of reflection from platform
    @ PLATFORM_MAX_ANGLE: -20       , maximum allowed angle of reflection from platform
    @ reflection = (ball_x - platform_x) / 16 pixels * 32 degrees
    @            = (ball_x - platform_x) *2
    @ ball_heading = limit_angle(-ball_heading + reflection)

    sub   r5, r1, r2       @ r5 = ball_x - platform_x
    mov   r5, r5, lsl #1   @ r5 *= 2
    rsb   r0, r0, r5       @ r0 = r5 - r0 (reflection)
    bl    limit_angle
    cmp   r0, #-160        @ if (reflection < -160)
    movlt r0, #-160        @ ball_heading = -160
    blt   end
    cmp   r0, #90          @ else if (reflection > 90)
    movgt r0, #-160        @ ball_heading = -160
    bgt   end
    cmp   r0, #-20         @ else if (reflection > -20)
    movgt r0, #-20         @ ball_heading = -20
    bgt   end

end:

    @ Exit from function
    ldmfd   sp!, {r4-r11, lr}     @ Recover past state of r4-r11 and link register from sp register
    pop {pc}            @ restore the link register and return to the caller
