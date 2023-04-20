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

@ HALF_PLATFORM_WIDTH, 8        @ width of half of platform
@ PLATFORM_MAX_DEFLECTION, 32   @ maximum additional deflection by hitting end of platform
@ PLATFORM_MIN_ANGLE, -160      @ minimum allowed angle of reflection from platform
@ PLATFORM_MAX_ANGLE, -20       @ maximum allowed angle of reflection from platform

.global platform_top_deflection
platform_top_deflection:
    @ r0: ball center x coordinate
    @ r1: platform x coordinate
    @ r2: ball heading
    push {lr}           @ save the link register
    stmfd   sp!, {r4-r11, lr}     @ save content of r4-r11 and link register into the sp register

    add   r3, r1, #8       @ r3 = platform_x + 8
    sub   r4, r1, #8       @ r4 = platform_x - 8
    cmp   r0, r3           @ if (ball_x < r3)
    cmplt r0, r4           @ && if (ball_x >= r4)
    bge   reflect          @ both conditions satisfy, reflect
    b     game_end         @ else, no reflect

reflect:
    @ find additional angle to deflect ball at
    @ reflection = (ball_x - platform_x) / 16 pixels * 32 degrees
    @            = (ball_x - platform_x) *2
    @ ball_heading = -ball_heading + reflection

    sub   r5, r0, r1       @ r5 = ball_x - platform_x
    mov   r5, r5, lsl #1   @ r5 *= 2
    rsb   r5, r2, r5       @ r5 = r5 - r2
    mov   r0, r5           @ r0 = r5 (reflection)
    bl    limit_angle
    cmp   r0, #-160        @ if (reflection < -160)
    movlt r0, #-160        @ ball_heading = -160
    blt   game_end
    cmp   r0, #90          @ else if (reflection > 90)
    movgt r0, #-160        @ ball_heading = -160
    bgt   game_end
    cmp   r0, #-20         @ else if (reflection > -20)
    movgt r0, #-20         @ ball_heading = -20
    bgt   game_end

game_end:

    @ Exit from function
    ldmfd   sp!, {r4-r11, lr}     @ Recover past state of r4-r11 and link register from sp register
    pop {pc}            @ restore the link register and return to the caller
